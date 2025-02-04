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
    TreeSolver() 
    {
        std::ifstream input("input.txt");

        input >> nodeCount;

        for(int i = 0; i < nodeCount - 1; ++i)
        {
            int first;
            int second;

            input >> first;
            input >> second;

            adj[first].push_back(second);
            adj[second].push_back(first);
        }

        costs.resize(nodeCount + 1, 0);

        for(int i = 1; i <= nodeCount; ++i)
        {
            input >> costs[i];
        }

        dp.resize(nodeCount + 1, std::vector<int64_t>(2, -1));
    }

    void Solve()
    {
        if(nodeCount == 1) 
        { 
            std::cout << costs[1] << " " << 1 << std::endl << "1"; 
            return;
        }

        adj[0].push_back(1);
        int64_t minCost = SolveDFS(0, 0, true);
        RecoverStateDFS(0, 0, true);

        std::cout << minCost << " " << marked.size() << std::endl;

        for(int i = 0; i < marked.size(); ++i)
        {
            if(i != 0) { std::cout << " "; }

            std::cout << marked[i];
        }
    }

private:
    int64_t SolveDFS(int node, int parent, bool toMark)
    {
        if(dp[node][toMark] != -1) { return dp[node][toMark]; }

        int64_t res = 0;
        NodeList const& children = adj[node];

        for(int child : children)
        {
            if(child == parent) { continue; }

            if(toMark)
            {
                res += std::min(SolveDFS(child, node, false), SolveDFS(child, node, true));
            }
            else
            {
                res += SolveDFS(child, node, true);
            }
        }

        res += toMark ? costs[node] : 0;
        dp[node][toMark] = res;

        return res;
    }

    void RecoverStateDFS(int node, int parent, bool toMark)
    {
        int64_t res = 0;
        NodeList const& children = adj[node];

        for(int child : children)
        {
            if(child == parent) { continue; }

            if(!toMark || dp[child][true] < dp[child][false])
            {
                marked.push_back(child);
                RecoverStateDFS(child, node, true);
            }
            else
            {
                RecoverStateDFS(child, node, false);
            }
        }
    }

private:
    NodeList marked;
    std::vector<std::vector<int64_t>> dp;
    std::vector<int> costs;
    AdjacencyMap adj;
    int nodeCount;
};

int main()
{
    TreeSolver solver;
    solver.Solve();

    return 0;
}