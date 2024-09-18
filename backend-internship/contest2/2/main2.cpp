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
    std::ofstream output("output.txt");

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
    
    for(int day = 0; day < dayCount; ++day)
    {
        int price;
        input >> price;

        prices[day] = price;

        while (!nextMinimum.empty() 
            && nextMinimum.back().day > day - slidingWindowSize
            && nextMinimum.back().value > price)
        {
            nextMinimum.pop_back();
        }

        nextMinimum.emplace_back(day, price);
    }

    for(int day = 0; day < dayCount; ++day)
    {
        Price const& nextMinPrice = nextMinimum.front();

        int fishBought = 0;

        if(nextMinPrice.day == day)
        {
            fishBought = std::min(slidingWindowSize, dayCount - day) - fishCount;
            sum += fishBought * (int64_t)nextMinPrice.value;
            nextMinimum.pop_front();
        }
        else if(fishCount == 0)
        {
            fishBought = 1;
            sum += fishBought * (int64_t)prices[day];
        }

        fishBoughtByDay[day] = fishBought;
        fishCount += fishBought - 1;
    }

    output << sum << "\n";
    std::ranges::copy(fishBoughtByDay, std::ostream_iterator<int>(output, " "));

    return 0;
}