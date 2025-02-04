#include <fstream>
#include <cstdint>
#include <vector>
#include <limits>
#include <algorithm>
#include <queue>
#include <cmath>

struct Chair
{
    int height;
    int width;

    bool operator< (Chair const& rhs) const
    {
        return this->height < rhs.height;
    }
};

struct DequeEntry
{
    int index;
    int maxValue;

    DequeEntry(int index, int maxValue)
        : index(index), maxValue(maxValue) {}
};

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int count;
    int minWidth;

    input >> count;
    input >> minWidth;

    std::vector<Chair> chairs(count);

    for(int i = 0; i < count; ++i)
    {
        input >> chairs[i].height;
    }

    for(int i = 0; i < count; ++i)
    {
        input >> chairs[i].width;
    }

    std::sort(chairs.begin(), chairs.end());

    std::deque<DequeEntry> nextMaxComfort;
    uint64_t totalWidth = 0;
    int minComfort = (std::numeric_limits<int>::max)();
    int left = 0;

    for(int right = 0; right < count; ++right)
    {
        totalWidth += chairs[right].width;

        while(left < right && totalWidth - chairs[left].width >= minWidth)
        {
            totalWidth -= chairs[left].width;

            if(!nextMaxComfort.empty() && nextMaxComfort.front().index == left)
            {
                nextMaxComfort.pop_front();
            }

            ++left;
        }

        if(left < right)
        {
            int newMaxComfort = abs(chairs[right].height - chairs[right - 1].height);

            while(!nextMaxComfort.empty() && newMaxComfort >= nextMaxComfort.back().maxValue)
            {
                nextMaxComfort.pop_back();
            }

            nextMaxComfort.emplace_back(right - 1, newMaxComfort);
        }

        if(totalWidth >= minWidth)
        {
            int currentComfort = nextMaxComfort.empty() ? 0 : nextMaxComfort.front().maxValue;
            minComfort = std::min(minComfort, currentComfort);
        }
    }

    output << minComfort;

    return 0;
}