#include <fstream>
#include <unordered_map>
#include <vector>
#include <iterator>
#include <ranges>
#include <algorithm>

int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int maxCount = 0;
    std::unordered_map<int, int> numberCount;

    int character = input.peek();

    while(character != EOF) 
    { 
        switch(character)
        {
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            {
                int number;
                input >> number;

                numberCount[number] += 1;
                maxCount = std::max(maxCount, numberCount[number]);

                break;
            }
            default:
                input.get();
        }

        character = input.peek();        
    }

    std::vector<int> result;

    for(auto const&[number, count] : numberCount) 
    {
        if(count == maxCount)
        {
            result.push_back(number);
        }
    }
    
    std::ranges::sort(result);
    std::ranges::copy(result, std::ostream_iterator<int>(output, " "));

    return 0;
}
