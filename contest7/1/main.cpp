#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int funcCount;
    input >> funcCount;

    std::unordered_map<std::string, std::vector<std::string>> dependents;
    std::unordered_map<std::string, int> dependencyLevel;
    std::queue<std::string> queue;

    for(int i = 0; i < funcCount; ++i)
    {
        std::string funcName;
        int dependencyCount;

        input >> funcName;
        input >> dependencyCount;

        if(dependencyCount == 0) 
        { 
            queue.push(funcName); 
        }
        else
        {
            dependencyLevel[funcName] = dependencyCount;
        }

        for(int j = 0; j < dependencyCount; ++j) 
        {
            std::string dependencyFuncName;
            input >> dependencyFuncName;
            
            dependents[dependencyFuncName].push_back(funcName);
        }
    }

    while(!queue.empty()) 
    {
        std::string nextFunc = queue.front();
        output << nextFunc << " ";

        queue.pop();

        if(dependents.count(nextFunc) == 0)
        {
            continue;
        }

        std::vector<std::string> const& funcsToUpdate = dependents.at(nextFunc);

        for(auto const& func : funcsToUpdate)
        {
            --dependencyLevel.at(func);

            if(dependencyLevel.at(func) == 0)
            {
                queue.push(func);
            }
        }
        
    }

    return 0;
}