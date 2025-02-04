#include <fstream>
#include <vector>

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int size;
    int targetSum;

    input >> size;
    input >> targetSum;

    int sum = 0;
    std::vector<int> prefix(size + 1, 0);

    for(int i = 0; i < size; ++i)
    {
        int value;
        input >> value;

        sum += value;
        prefix[i + 1] = sum;
    }

    int count = 0;
    int left = 0;

    for(int right = 1; right <= size; ++right)
    {
        int sum = prefix[right] - prefix[left];

        while(left < right - 1 && sum > targetSum)
        {
            ++left;
            sum = prefix[right] - prefix[left];
        }

        if(sum == targetSum) { ++count; }
    }

    output << count;

    return 0;
}