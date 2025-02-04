#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iterator>

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int size;

    input >> size;

    std::vector<int> numbers(size, 0);

    for(int i = 0; i < size; ++i)
    {
        input >> numbers[i];
    }

    std::ranges::sort(numbers);
    
    auto firstIt = numbers.rbegin() + size / 2;
    auto secondIt = firstIt.base();
    int count = 0;

    while(count < size)
    {
        int toPrint = 0;
        int distFirst = std::distance(firstIt, numbers.rend());
        int distSecond = std::distance(secondIt, numbers.end());

        if(distFirst < distSecond)
        {
            toPrint = *secondIt;
            ++secondIt; 
        }
        else if(distFirst > distSecond)
        {
            toPrint = *firstIt;
            ++firstIt; 
        }
        else
        {
            if(*firstIt < *secondIt)
            {
                toPrint = *firstIt;
                ++firstIt; 
            }
            else
            {
                toPrint = *secondIt;
                ++secondIt;    
            }
        }

        if(count > 0) { output << " "; }

        output << toPrint;
        ++count;
    }

    return 0;
}