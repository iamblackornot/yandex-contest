#include <fstream>
#include <vector>

int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int count;
    input >> count;

    if(count & 1)
    {
        output << -1;
        return 0;
    }

    std::vector<int> numbers(count);

    for(int i = 0; i < count; ++i)
    {
        input >> numbers[i];
    }

    int targetSum = numbers[0] + numbers[numbers.size() - 1];
    int half = count / 2;

    for(int i = 1; i < half; ++i)
    {
        int currSum = numbers[i] + numbers[numbers.size() - i - 1];

        if(currSum != targetSum)
        {
            output << -1;
            return 0;
        }
    }

    output << targetSum;

    return 0;
}