#include <cstdint>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <vector>

typedef std::unordered_map<std::string, std::string> AdjacencyMap;

class TreeSolver
{
public:
    TreeSolver() 
    {
        std::ifstream input("input.txt");

        input >> nodeCount;
        
        for(int i = 0; i < nodeCount - 1; ++i)
        {
            std::string first;
            std::string second;

            input >> first;
            input >> second;

            adj[first] = second;

            uniqueNames.insert(first);
            uniqueNames.insert(second);
        }
    }

    void Solve()
    {
        for(std::string const& name : uniqueNames)
        {
            SolveDFS(name);
        }

        for(auto const& [name, height] : heights)
        {
            std::cout << name << " " << height << std::endl;
        }
    }

private:
    int SolveDFS(std::string const& node)
    {
        if(heights.contains(node))
        {
            return heights.at(node);
        }

        if(!adj.contains(node))
        {
            heights[node] = 0;
            return 0;
        }


        int height = 1 + SolveDFS(adj.at(node));
        heights[node] = height;
        return height;
    }

private:
    std::unordered_set<std::string> uniqueNames;
    std::map<std::string, int> heights;
    AdjacencyMap adj;
    int nodeCount;
};

int main()
{
    TreeSolver solver;
    solver.Solve();

    return 0;
}