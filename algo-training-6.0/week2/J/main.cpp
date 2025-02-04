#include <fstream>
#include <vector>
#include <cstdint>

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int size;
    input >> size;

    std::vector<int> weight(size, 0);

    for(int i = 0; i < size; ++i)
    {
        input >> weight[i];
    }

    int casesCount;
    int threshold;

    input >> casesCount;
    input >> threshold;

    std::vector<uint64_t> sortedUntil(size, 0);

    for(int i = 1; i < size; ++i)
    {
        sortedUntil[i] = weight[i - 1] <= weight[i] ? sortedUntil[i - 1] : i;
    }

    int right = size - 1;
    int duplicates = 0;
    std::vector<uint64_t> underThresholdUntil(size, 0);

    for(int left = right - 1; left >= 0; --left)
    {
        if(weight[left + 1] == weight[left]) { ++duplicates; }

        while(duplicates > threshold)
        {
            underThresholdUntil[right] = 1 + left;
            --right;

            if(weight[right + 1] == weight[right]) { --duplicates; }
        }
    }

    for(int i = 0; i < casesCount; ++i)
    {
        int index = 0;
        input >> index;
        --index;

        if(i > 0) { output << " "; }
        output << std::max(sortedUntil[index] + 1, underThresholdUntil[index] + 1);
    }

    return 0;
}