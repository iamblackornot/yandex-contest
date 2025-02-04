#include <fstream>
#include <vector>
#include <cstdint>

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int size;
    int visibilityRange;

    input >> size;
    input >> visibilityRange;

    int sum = 0;
    std::vector<int> distance(size, 0);

    for(int i = 0; i < size; ++i)
    {
        input >> distance[i];
    }

    uint64_t count = 0;
    int right = 1;

    for(int left = 0; left < size && right < size; ++left)
    {
        while(right < size && (distance[right] - distance[left] <= visibilityRange))
        {
            ++right;
        }

        count += size - right;
    }

    output << count;

    return 0;
}