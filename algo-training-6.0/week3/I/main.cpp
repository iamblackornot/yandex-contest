#include <fstream>
#include <cstdint>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <queue>

int nextLane(int current)
{
    return (current + 1) % 4;
}

struct Rover
{
    int index;
    int startLane;
    int startTime;

    bool operator< (Rover const& rhs) const
    {
        return this->startTime < rhs.startTime;
    }
};

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int roversCount;
    input >> roversCount;

    int mainStart;
    int mainEnd;

    input >> mainStart;
    input >> mainEnd;

    std::vector<Rover> rovers(roversCount);
    int minTime = (std::numeric_limits<int>::max)();

    for(int i = 0; i < roversCount; ++i)
    {
        rovers[i].index = i;
        input >> rovers[i].startLane;
        input >> rovers[i].startTime;

        minTime = std::min(minTime, rovers[i].startTime);
    }

    std::sort(rovers.begin(), rovers.end());
    std::queue<Rover> queues[4];

    for(int i = 0; i < roversCount; ++i)
    {
        Rover const& rover = rovers[i];
        queues[rover.startLane - 1].push(rover);
    }

    if(mainStart > mainEnd) { std::swap(mainStart, mainEnd); }

    bool isMainStraight =   (mainStart == 1 && mainEnd == 3)
                         || (mainStart == 2 && mainEnd == 4);
    
    std::vector<int> priority(4);

    if(isMainStraight)
    {
        priority[0] = mainStart - 1;
        priority[1] = mainEnd - 1;
        priority[2] = nextLane(mainStart - 1);
        priority[3] = nextLane(mainEnd - 1);
    }
    else
    {
        int start = (mainEnd == 4 && mainStart == 1 ? mainEnd - 1 : mainStart - 1);

        for(int i = 0; i < 4; ++i)
        {
            priority[i] = start;
            start = nextLane(start);
        }
    }

    int time = minTime;
    bool areQueuesEmpty = false;
    std::vector<int> result(roversCount);

    while(!areQueuesEmpty)
    {
        if(isMainStraight)
        {
            bool roverMoved = false;

            int mainA = priority[0];
            int mainB = priority[1];

            if(!queues[mainA].empty() && queues[mainA].front().startTime <= time)
            {
                result[queues[mainA].front().index] = time;
                queues[mainA].pop();
                roverMoved = true;
            }

            if(!queues[mainB].empty() && queues[mainB].front().startTime <= time)
            {
                result[queues[mainB].front().index] = time;
                queues[mainB].pop();
                roverMoved = true;
            }

            if(!roverMoved)
            {
                int secA = priority[2];
                int secB = priority[3];

                if(!queues[secA].empty() && queues[secA].front().startTime <= time)
                {
                    result[queues[secA].front().index] = time;
                    queues[secA].pop();
                }

                if(!queues[secB].empty() && queues[secB].front().startTime <= time)
                {
                    result[queues[secB].front().index] = time;
                    queues[secB].pop();
                }
            }  
        }
        else
        {
            bool roverMoved = false;

            for(int i = 0; i < 4 && !roverMoved; ++i)
            {
                int lane = priority[i];

                if(!queues[lane].empty() && queues[lane].front().startTime <= time)
                {
                    result[queues[lane].front().index] = time;
                    queues[lane].pop();
                    roverMoved = true;
                }
            }
        }

        ++time;
        areQueuesEmpty = queues[0].empty() && queues[1].empty() && queues[2].empty() && queues[3].empty();
    }

    for(int i = 0; i < roversCount; ++i)
    {
        output << result[i] << std::endl;
    }

    return 0;
}
