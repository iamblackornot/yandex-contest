#include <fstream>
#include <vector>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <numeric>
#include <queue>

constexpr int INTERESTING = 0;
constexpr int USEFUL = 1;

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int size;
    input >> size;

    std::vector<int> rooms(size, 0);
    std::vector<int> params[2]
    {
        std::vector(size, 0),
        std::vector(size, 0)
    };

    auto interestingCompare = [&](int first, int second)
    {
        if(params[INTERESTING][first] != params[INTERESTING][second]) 
            return params[INTERESTING][first] < params[INTERESTING][second];
        
        if(params[USEFUL][first] != params[USEFUL][second])
            return params[USEFUL][first] < params[USEFUL][second];

        return first > second;
    };

    auto usefulCompare = [&](int first, int second)
    {
        if(params[USEFUL][first] != params[USEFUL][second])
            return params[USEFUL][first] < params[USEFUL][second];

        if(params[INTERESTING][first] != params[INTERESTING][second]) 
            return params[INTERESTING][first] < params[INTERESTING][second];

        return first > second;
    };

    std::priority_queue<int, std::vector<int>, decltype(interestingCompare)> interestingQueue(interestingCompare);
    std::priority_queue<int, std::vector<int>, decltype(usefulCompare)> usefulQueue(usefulCompare);

    std::vector<int> algoType (size, 0);
    std::vector<bool> done (size, false);

    for(int i = 0; i < size; ++i)
    {
        input >> params[INTERESTING][i];
    }

    for(int i = 0; i < size; ++i)
    {
        input >> params[USEFUL][i];
    }

    for(int i = 0; i < size; ++i)
    {
        input >> algoType[i];
    }

    for(int i = 0; i < size; ++i)
    {
        interestingQueue.push(i);
        usefulQueue.push(i);
    }

    for(int i = 0; i < size; ++i)
    {
        if(i > 0) { output << " "; }

        if(algoType[i] == INTERESTING)
        {
            while(done[interestingQueue.top()])
            {
                interestingQueue.pop();
            }

            done[interestingQueue.top()] = true;
            output << interestingQueue.top() + 1;
            interestingQueue.pop();
        }
        else
        {
            while(done[usefulQueue.top()])
            {
                usefulQueue.pop();
            }

            done[usefulQueue.top()] = true;
            output << usefulQueue.top() + 1;
            usefulQueue.pop();
        }
    }

    return 0;
}