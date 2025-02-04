#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iterator>

constexpr int MODULO = 1'000'000'007;

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

    uint64_t sum1 = 0;
    uint64_t sum2 = 0;
    uint64_t sumTotal = 0;

    for(int i = 0; i < size - 2; ++i)
    {
        int revIndex = size - 2 - i - 1;

        sum1 = (sum1 + numbers[revIndex + 2]) % MODULO;
        sum2 = (sum2 + numbers[revIndex + 1] * sum1) % MODULO;
        sumTotal = (sumTotal + numbers[revIndex] * sum2) % MODULO;
    }

    output << sumTotal;

    return 0;
}