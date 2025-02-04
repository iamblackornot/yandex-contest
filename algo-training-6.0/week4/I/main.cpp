#include <cstdint>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <vector>
#include <queue>

typedef std::vector<int> NodeList;
typedef std::unordered_map<int, NodeList> AdjacencyMap;

class TreeSolver
{
public:
    TreeSolver(AdjacencyMap&& adj, int nodeCount) :
        adj(std::move(adj)), nodeCount(nodeCount) {}

    void Solve()
    {
        int farthest = FindFarthestNode(1);
        FindFarthestNodePath(farthest);

        int maxOutsidePath = 0;
        std::vector<int> prefixMax(diameterPath.size() + 1, 0);

        for(int i = 0; i < diameterPath.size(); ++i)
        {
            int node = diameterPath[i];
            NodeList const& children = adj[node];
            int maxOutsideDepth = 0;

            for(int child : children)
            {
                if(diameterNodes.contains(child)) { continue; }

                MaxDepthAndPathDFS(child, node);
                maxOutsidePath = std::max(maxOutsidePath, maxPath[child]);
                maxOutsideDepth = std::max(maxOutsideDepth, 1 + maxDepth[child]);
            }

            prefixMax[i + 1] = std::max(prefixMax[i], i + maxOutsideDepth);
        }

        int revMaxSoFar = 0;
        uint64_t res = 0;

        for(int i = 0; i < diameterPath.size() - 1; ++i)
        {
            int revIndex = diameterPath.size() - i - 1;
            int node = diameterPath[revIndex];
            NodeList const& children = adj[node];
            int maxOutsideDepth = 0;

            for(int child : children)
            {
                if(diameterNodes.contains(child)) { continue; }

                maxOutsideDepth = std::max(maxOutsideDepth, 1 + maxDepth[child]);
            }

            revMaxSoFar = std::max(revMaxSoFar, i + maxOutsideDepth);
            res = std::max(res, (uint64_t)revMaxSoFar * (uint64_t)prefixMax[revIndex]);
        }

        res = std::max(res, (diameterPath.size() - 1) * maxOutsidePath);
        std::cout << res;
    }
private:
    void MaxDepthAndPathDFS(int node, int parent)
    {
        int maxSubtreePath = 0;
        std::multiset<int> subtreeDepth { 0 };

        NodeList const& children = adj[node];

        for(int child : children)
        {
            if(child == parent) continue;

            MaxDepthAndPathDFS(child, node);

            subtreeDepth.insert(1 + maxDepth[child]);

            if(subtreeDepth.size() > 2) { subtreeDepth.erase(subtreeDepth.begin()); }

            maxSubtreePath = std::max(maxSubtreePath, maxPath[child]);
        }

        maxDepth[node] = *subtreeDepth.rbegin();

        int currMaxPath = 0;
        auto it = subtreeDepth.begin();

        while(it != subtreeDepth.end())
        {
            currMaxPath += *it;
            ++it;
        }

        maxPath[node] = std::max(maxSubtreePath, currMaxPath);
    }

    int FindFarthestNode(int start)
    {
        std::vector<int> parent(nodeCount + 1, -1);
        std::vector<int> dist(nodeCount + 1, -1);
        std::queue<int> queue;

        int farthest = start;
        dist[start] = 0;
        queue.push(start);

        while(!queue.empty())
        {
            int node = queue.front();
            queue.pop();

            auto const& children = adj[node];

            for(int child : children)
            {
                if(child == parent[node]) continue;

                queue.push(child);
                parent[child] = node;
                dist[child] = 1 + dist[node];
                farthest = dist[child] > dist[farthest] ? child : farthest;
            }
        }
        
        return farthest;
    }

    void FindFarthestNodePath(int start)
    {
        std::vector<int> parent(nodeCount + 1, -1);
        std::vector<int> dist(nodeCount + 1, -1);
        std::queue<int> queue;

        int farthest = start;
        dist[start] = 0;
        queue.push(start);

        while(!queue.empty())
        {
            int node = queue.front();
            queue.pop();

            auto const& children = adj[node];

            for(int child : children)
            {
                if(child == parent[node]) continue;

                queue.push(child);
                parent[child] = node;
                dist[child] = 1 + dist[node];
                farthest = dist[child] > dist[farthest] ? child : farthest;
            }
        }

        int node = farthest;

        while(node != -1)
        {
            diameterPath.push_back(node);
            diameterNodes.insert(node);
            node = parent[node];
        }
    }

    AdjacencyMap adj;
    int nodeCount;

    NodeList diameterPath;
    std::unordered_set<int> diameterNodes;

    std::unordered_map<int, int> maxPath;
    std::unordered_map<int, int> maxDepth;
};

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int nodeCount;
    input >> nodeCount;

    AdjacencyMap adj;

    for(int i = 0; i < nodeCount - 1; ++i)
    {
        int first;
        int second;

        input >> first;
        input >> second;

        adj[first].push_back(second);
        adj[second].push_back(first);
    }

    TreeSolver solver(std::move(adj), nodeCount);
    solver.Solve();

    return 0;
}