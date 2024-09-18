#include <fstream>
#include <vector>
#include <set>
#include <queue>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <format>
#include <climits>
#include <cstdint>

struct Line
{
    int origin;
    int dest;
    int ping;
    int cost;

    Line(int origin, int dest, int ping, int cost = 0);
};

typedef int LineId;
typedef std::reference_wrapper<const Line>           LineRef;
typedef std::unordered_map<LineId, int>              NewLineMap;
typedef std::unordered_map<int, std::vector<LineId>> AdjacencyMap;
typedef std::vector<Line>                            RequirementList;

struct NodeStat
{
    int nodeNum = 0;
    int totalPing = INT_MAX;
    int64_t totalCost = INT64_MAX;

    NodeStat() {};
    NodeStat(int nodeNum, int totalPing, int64_t totalCost)
        : nodeNum(nodeNum), totalPing(totalPing), totalCost(totalCost) {}

    bool operator<(NodeStat const& rhs) const
    {
        return std::tie(totalPing, totalCost) 
             > std::tie(rhs.totalPing, rhs.totalCost);
    }
};

struct NodeStatWithPath : public NodeStat
{
    std::vector<LineId> path;

    NodeStatWithPath(int nodeNum, int totalPing, int64_t totalCost, std::vector<LineId> path)
        : NodeStat(nodeNum, totalPing, totalPing), path(std::move(path)) {}

    bool operator<(NodeStatWithPath const& rhs) const
    {
        return std::tie(totalCost, totalPing) 
             > std::tie(rhs.totalCost, rhs.totalPing);
    }
};

typedef std::function<bool(
    NodeStat const& lhs, 
    NodeStat const& rhs)> NodeStatComparator;

inline bool operator==(Line const& lhs, Line const& rhs)
{
    return std::tie(lhs.origin, lhs.dest, lhs.ping, lhs.cost) 
        == std::tie(rhs.origin, rhs.dest, rhs.ping, rhs.cost);
}

struct ProjectConclusion
{
    bool viable;
    std::vector<int> newLines;
};

struct OptimalPath
{
    std::unordered_set<int> newLines;
    NodeStat stat;
    int requirementId;
    Line const& requirement;
    bool exists;
};

class NetworkManager
{
public:
    NetworkManager(int nodesCount);
    void AddLine(Line const& line);
    void AddRequirement(Line const& line);
    ProjectConclusion GetProjectAnalysis();
private:
    std::vector<int> GetTopologicalSort();
    OptimalPath GetPingOptimalPath(Line const& req);
    OptimalPath GetCostOptimalPath(Line const& req);
private:
    int nodesCount;
    std::vector<Line> lines;
    NewLineMap newLines;
    AdjacencyMap adjacency;
    RequirementList requirements;
};

Line ReadLine(std::istream& input);
Line ReadNewLine(std::istream& input);

int main()
{
    int nodesCount;
    int linesCount;

    std::ifstream input("input.txt");

    input >> nodesCount;
    input >> linesCount;

    NetworkManager manager(nodesCount);

    for(int i = 0; i < linesCount; ++i)
    {
        Line line = ReadLine(input);
        manager.AddLine(line);
    }

    int newLinesCount;
    input >> newLinesCount;

    for(int i = 0; i < newLinesCount; ++i)
    {
        Line line = ReadNewLine(input);
        manager.AddLine(line);
    }

    int requirementsCount;
    input >> requirementsCount;

    for(int i = 0; i < requirementsCount; ++i)
    {
        Line line = ReadLine(input);
        manager.AddRequirement(line);
    }

    ProjectConclusion const& conclusion = manager.GetProjectAnalysis();
    std::ofstream output("output.txt");

    if(!conclusion.viable)
    {
        output << -1;
        return 0;
    }

    output << conclusion.newLines.size() << "\n";
    std::ranges::copy(conclusion.newLines, std::ostream_iterator<int>(output, " "));

    return 0;
}

Line ReadLine(std::istream& input)
{
    int origin;
    int dest;
    int ping;

    input >> origin;
    input >> dest;
    input >> ping;

    return Line(--origin, --dest, ping);
}

Line ReadNewLine(std::istream& input)
{
    int origin;
    int dest;
    int ping;
    int cost;

    input >> origin;
    input >> dest;
    input >> ping;
    input >> cost;

    return Line(--origin, --dest, ping, cost);
}

inline Line::Line(int origin, int dest, int ping, int cost)
    : origin(origin), dest(dest), ping(ping), cost(cost) {}

inline NetworkManager::NetworkManager(int nodesCount) 
    : nodesCount(nodesCount) {}

void NetworkManager::AddLine(Line const& line)
{
    lines.push_back(line);
    LineId newLineId = lines.size() - 1;

    adjacency[line.origin].push_back(newLineId);
    adjacency[line.dest].push_back(newLineId);

    if(line.cost != 0) { newLines[newLineId] = newLines.size() + 1; }
}

void NetworkManager::AddRequirement(Line const &line)
{
    requirements.push_back(line);
}

OptimalPath NetworkManager::GetPingOptimalPath(Line const& req)
{
    std::vector<NodeStat> stat(nodesCount);
    std::vector<bool> visited(nodesCount, false);
    stat[req.origin].totalPing = 0;
    stat[req.origin].totalCost = 0;

    std::vector<LineId> path(nodesCount, -1);
    
    std::priority_queue<NodeStat> queue;
    queue.emplace(req.origin, 0, 0);

    while(!queue.empty())
    {
        int origin = queue.top().nodeNum;

        auto const& adjLines = adjacency[origin];

        for(int lineId : adjLines)
        {
            Line const& line = lines[lineId];
            int dest = line.origin == origin ? line.dest : line.origin;

            if(visited[dest]) { continue; }

            int newTotalPing = stat[origin].totalPing + line.ping;
            int64_t newTotalCost = stat[origin].totalCost + line.cost;
            
            bool toUpdate = newTotalPing == stat[dest].totalPing 
                          ? newTotalCost < stat[dest].totalCost
                          : newTotalPing < stat[dest].totalPing; 
            
            if(toUpdate) 
            {
                stat[dest] = NodeStat(dest, newTotalPing, newTotalCost);
                path[dest] = lineId;
                queue.emplace(dest, newTotalPing, newTotalCost);
            }
        }

        visited[origin] = true;
        queue.pop();
    }

    OptimalPath res = OptimalPath{ 
        .stat = stat[req.dest],
        .requirement = req,
        .exists = true
    };

    int currNode = req.dest;
    LineId nextLineId = path[currNode];

    if(nextLineId < 0)
    {
        res.exists = false;
        return res;
    }

    while(nextLineId != -1)
    {
        if(lines[nextLineId].cost > 0) { res.newLines.insert(nextLineId); }

        currNode = lines[nextLineId].dest == currNode ? lines[nextLineId].origin : lines[nextLineId].dest;
        nextLineId = path[currNode];
    }

    return res;
}

OptimalPath NetworkManager::GetCostOptimalPath(Line const& req)
{   
    std::priority_queue<NodeStatWithPath> queue;
    queue.emplace(req.origin, 0, 0, std::vector<LineId>{});

    std::vector<LineId> resPath;

    OptimalPath res = OptimalPath{
        .requirement = req,
        .exists = false
    };

    while(!queue.empty())
    {
        NodeStatWithPath const& stat = queue.top();
        int origin = stat.nodeNum;

        if(origin == req.dest) 
        {
            resPath = stat.path;
            res.stat = stat;
            res.exists = true;
        }

        auto const& adjLines = adjacency[origin];

        for(int lineId : adjLines)
        {
            Line const& line = lines[lineId];
            int dest = line.origin == origin ? line.dest : line.origin;

            int newTotalPing = stat.totalPing + line.ping;
            int64_t newTotalCost = stat.totalCost + line.cost;
            
            if(newTotalPing <= req.ping)
            {
                auto newPath = stat.path;
                newPath.push_back(lineId);
                queue.emplace(dest, newTotalPing, newTotalCost, std::move(newPath));
            }
        }

        queue.pop();
    }

    if(!res.exists) { return res; }

    auto it = resPath.rbegin();
    int currNode = req.dest;

    while(it != resPath.rend())
    {
        LineId lineId = *it;
        if(lines[lineId].cost > 0) { res.newLines.insert(lineId); }

        currNode = lines[lineId].dest == currNode ? lines[lineId].origin : lines[lineId].dest;
        ++it;
    }

    return res;
}

ProjectConclusion NetworkManager::GetProjectAnalysis()
{
    std::vector<OptimalPath> projects;
    std::set<LineId> linesToBuild;

    for(int i = 0; i < requirements.size(); ++i)
    {      
        projects.push_back(GetPingOptimalPath(requirements[i]));

        if(!projects[i].exists) { return ProjectConclusion { .viable = false }; }
        if(projects[i].stat.totalPing > requirements[i].ping) { return ProjectConclusion { .viable = false }; }

        linesToBuild.insert(projects[i].newLines.begin(), projects[i].newLines.end());
    }

    if(linesToBuild.empty()) { return ProjectConclusion { .viable = true }; }

    LineId mostExpensiveLine = -1;
    int mostExpensiveCost = -1;

    for(LineId lineId : linesToBuild)
    {
        if(lines[lineId].cost > mostExpensiveCost)
        {
            mostExpensiveLine = lineId;
            mostExpensiveCost = lines[lineId].cost;
        }
    }

    int pingRoomForMinimizing = INT_MAX;

    for(OptimalPath const& project : projects)
    {
        if(project.newLines.contains(mostExpensiveLine))
        {
            pingRoomForMinimizing = std::min(
                pingRoomForMinimizing, 
                project.requirement.ping - project.stat.totalPing);
        }
    }

    Line minCostReq = Line(
        lines[mostExpensiveLine].origin, 
        lines[mostExpensiveLine].dest, 
        lines[mostExpensiveLine].ping + pingRoomForMinimizing,
        lines[mostExpensiveLine].cost);

    OptimalPath minCostProject = GetCostOptimalPath(minCostReq);

    if(minCostProject.exists)
    {
        linesToBuild.erase(mostExpensiveLine);
        linesToBuild.insert(minCostProject.newLines.begin(), minCostProject.newLines.end());
    }

    ProjectConclusion conclusion { .viable = true };

    for(LineId lineId : linesToBuild) 
    { 
        conclusion.newLines.push_back(newLines[lineId]);
    }

    return conclusion;
}