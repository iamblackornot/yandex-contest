#include <cstdint>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <vector>

typedef std::vector<std::string> NodeList;
typedef std::unordered_map<std::string, NodeList> AdjacencyMap;

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

            adj[second].push_back(first);

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

        for(auto const& [name, size] : sizes)
        {
            std::cout << name << " " << size << std::endl;
        }
    }

private:
    int SolveDFS(std::string const& node)
    {
        if(sizes.contains(node))
        {
            return sizes.at(node);
        }

        if(!adj.contains(node))
        {
            sizes[node] = 0;
            return 0;
        }

        int sum = 0;

        for(std::string const& child : adj.at(node))
        {
            sum += 1 + SolveDFS(child);
        }

        sizes[node] = sum;
        return sum;
    }

private:
    std::unordered_set<std::string> uniqueNames;
    std::map<std::string, int> sizes;
    AdjacencyMap adj;
    int nodeCount;
};

int main()
{
    TreeSolver solver;
    solver.Solve();

    return 0;
}