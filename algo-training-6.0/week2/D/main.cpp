#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int size;
    int threshold;

    input >> size;
    input >> threshold;

    std::vector<int> tasks(size, 0);

    for(int i = 0; i < size; ++i)
    {
        input >> tasks[i];
    }

    std::ranges::sort(tasks);

    int maxWindow = 0;
    int left = 0;

    for(int right = 0; right < size; ++right)
    {
        while(left < right && (tasks[right] - tasks[left] > threshold))
        {
            ++left;
        }

        maxWindow = std::max(maxWindow, right - left + 1);
    }

    output << maxWindow;

    return 0;
}