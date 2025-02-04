#include <fstream>
#include <vector>
#include <cstdint>

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int size;
    uint64_t threshold;

    input >> size;
    input >> threshold;

    std::string str;
    input >> str;

    int maxLength = 0;
    uint64_t combs = 0;
    int aCount = 0;
    int bCount = 0;
    int left = 0;

    for(int right = 0; right < size; ++right)
    {
        if(str[right] == 'a')
        {
            ++aCount;
        }
        else if(str[right] == 'b')
        {
            ++bCount;
            combs += aCount;
        }

        while(left < right && combs > threshold)
        {
            if(str[left] == 'a')
            {
                --aCount;
                combs -= bCount;
            }
            else if(str[left] == 'b')
            {
                --bCount;
            }

            ++left;
        }

        maxLength = std::max(maxLength, right - left + 1);
    }

    output << maxLength;

    return 0;
}