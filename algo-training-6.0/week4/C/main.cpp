#include <cstdint>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <sstream>

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
        }

        std::string str;
        std::getline(input, str);

        while(std::getline(input, str))
        {
            std::stringstream ss(str);

            std::string first;
            std::string second;

            ss >> first;
            ss >> second;

            std::vector<std::string> firstPath;
            std::vector<std::string> secondPath;

            SolveDFS(first, firstPath);
            SolveDFS(second, secondPath);

            auto firstIt = firstPath.rbegin();
            auto secondIt = secondPath.rbegin();

            while (firstIt != firstPath.rend() 
                && secondIt != secondPath.rend()
                && *firstIt == *secondIt)
            {
                ++firstIt;
                ++secondIt;
            }

            std::cout << *std::prev(firstIt) << std::endl;
        }
    }
private:
    void SolveDFS(std::string const& node, std::vector<std::string>& path)
    {
        path.push_back(node);

        if(!adj.contains(node))
        {
            return;
        }

        SolveDFS(adj.at(node), path);
    }

private:
    AdjacencyMap adj;
    int nodeCount;
};

int main()
{
    TreeSolver solver;

    return 0;
}