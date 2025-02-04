#include <fstream>
#include <cstdint>
#include <vector>
#include <string>

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int size;
    input >> size;
    
    std::vector<uint64_t> prefixSum{ 0 };

    for(int i = 0; i < size; ++i)
    {
        std::string command;
        input >> command;

        if(command[0] == '+')
        {
            int value = std::atoi(command.c_str() + 1);
            prefixSum.push_back(prefixSum.back() + value);
        }
        else if(command[0] == '-')
        {
            output << prefixSum.back() - prefixSum[prefixSum.size() - 2] << std::endl;
            prefixSum.pop_back();
        }
        else if(command[0] == '?')
        {
            int value = std::atoi(command.c_str() + 1);
            output << prefixSum.back() - prefixSum[prefixSum.size() - value - 1] << std::endl;
        }
    }

    return 0;
}