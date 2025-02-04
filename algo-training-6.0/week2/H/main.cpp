#include <fstream>
#include <vector>
#include <cstdint>
#include <limits>

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int size;
    input >> size;

    std::vector<int> rooms(size, 0);
    std::vector<uint64_t> prefix(size + 1, 0);
    std::vector<uint64_t> revPrefix(size + 1, 0);
    uint64_t sum = 0;

    for(int i = 0; i < size; ++i)
    {
        input >> rooms[i];
        sum += rooms[i];
        prefix[i + 1] = prefix[i] + sum;
    }

    sum = 0;

    for(int i = 0; i < size; ++i)
    {
        sum += rooms[size - i - 1];
        revPrefix[i + 1] = revPrefix[i] + sum;
    }

    uint64_t minSum = (std::numeric_limits<uint64_t>::max)();

    for(int i = 0; i < size; ++i)
    {
        minSum = std::min(minSum, prefix[i] - prefix[0] + revPrefix[size - i - 1] - revPrefix[0]);
    }

    output << minSum;

    return 0;
}