#include <fstream>
#include <cstdint>

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int size;
    uint64_t clientsPerSecond;

    input >> size;
    input >> clientsPerSecond;

    uint64_t timeSpent = 0;
    uint64_t waiting = 0;

    for(int i = 0; i < size; ++i)
    {
        uint64_t incomingClients;
        input >> incomingClients;

        uint64_t allClients = waiting + incomingClients;
        timeSpent += allClients;
        waiting = allClients - std::min(clientsPerSecond, allClients); 
    }

    timeSpent += waiting;
    output << timeSpent;

    return 0;
}

