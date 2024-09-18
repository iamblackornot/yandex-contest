#include <fstream>
#include <vector>
#include <iterator>

int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    std::vector<std::string> strings(2);

    size_t length, pairCount;
    input >> length;
    input >> pairCount;

    constexpr size_t FIRST = 0;
    constexpr size_t SECOND = 1;

    for(size_t pairNum = 0; pairNum < pairCount; ++pairNum)
    {
        input >> strings[FIRST];
        input >> strings[SECOND];

        size_t indexToTake = FIRST;
        std::ostreambuf_iterator<char> outIt(output);

        for(size_t charNum = 0; charNum < length; ++charNum)
        {
            if(strings[FIRST][charNum] == strings[SECOND][charNum])
            {
                outIt = strings[FIRST][charNum];
            }
            else
            {
                outIt = strings[indexToTake][charNum];
                indexToTake = 1 - indexToTake;
            }
        }

        output << std::endl;
    }

    return 0;
}