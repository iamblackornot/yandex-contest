#include <cstdint>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>

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

        sizes.resize(nodeCount + 1, 0);
    }

    void Solve()
    {
        SolveDFS(1, 1);

        for(int i = 1; i <= nodeCount; ++i)
        {
            if(i != 1) { std::cout << " "; }

            std::cout << sizes[i];
        }
    }

private:
    int SolveDFS(int node, int parent)
    {
        int size = 1;
        NodeList const& children = adj[node];

        for(int child : children)
        {
            if(child == parent) { continue; }

            size += SolveDFS(child, node);
        }

        sizes[node] = size;
        return size;
    }

private:
    std::vector<int> sizes;
    AdjacencyMap adj;
    int nodeCount;
};

int main()
{
    TreeSolver solver;
    solver.Solve();

    return 0;
}