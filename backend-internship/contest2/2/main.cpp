#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <iterator>

struct Price
{
    Price(int day, int value) 
        : day(day), value(value) {}
    int day;
    int value;
};


int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output2.txt");

    int dayCount;
    int storagePeriod;

    input >> dayCount;
    input >> storagePeriod;

    std::deque<Price> nextMinimum;
    int fishCount = 0;
    int64_t sum = 0;

    std::vector<int> prices(dayCount, 0);
    std::vector<int> fishBoughtByDay(dayCount, 0);

    int slidingWindowSize = std::min(storagePeriod, dayCount);
    
    for(int day = 0; day < dayCount + slidingWindowSize - 1; ++day)
    {
        if(day < dayCount)
        {
            int price;
            input >> price;

            prices[day] = price;

            while (!nextMinimum.empty() && nextMinimum.back().value > price)
            {
                nextMinimum.pop_back();
            }

            nextMinimum.emplace_back(day, price);
        }

        if(nextMinimum.front().day == day - slidingWindowSize)
        {
            nextMinimum.pop_front();
        }

        if(day >= slidingWindowSize - 1)
        {
            Price const& nextMinPrice = nextMinimum.front();

            int fishBought = 0;
            int buyDay = day - slidingWindowSize + 1;

            if(nextMinPrice.day == buyDay)
            {
                fishBought = std::min(slidingWindowSize, dayCount - buyDay) - fishCount;
                sum += fishBought * (int64_t)nextMinPrice.value;
            }
            else if(fishCount == 0)
            {
                fishBought = 1;
                sum += fishBought * (int64_t)prices[buyDay];
            }

            fishBoughtByDay[buyDay] = fishBought;
            fishCount += fishBought - 1;
        }
    }

    output << sum << "\n";
    std::ranges::copy(fishBoughtByDay, std::ostream_iterator<int>(output, " "));

    return 0;
}