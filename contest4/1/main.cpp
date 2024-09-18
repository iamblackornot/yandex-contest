#include <fstream>
#include <vector>
#include <numeric>

int main() 
{
    std::ifstream input("input.txt");

    int driversNum;
    input >> driversNum;
    
    std::vector<int> ratings;
    ratings.reserve(driversNum);

    for(size_t i = 0; i < driversNum; ++i)
    {
        int value;
        input >> value;

        ratings.push_back(value);
    }

    std::vector<int> payouts(driversNum, 5);

    for(size_t i = 1; i < driversNum; ++i)
    {
        if(ratings[i] > ratings[i - 1])
        {
            payouts[i] = 5 + payouts[i - 1];
        }
    }

    for(size_t i = driversNum - 1; i > 0; --i)
    {
        if(ratings[i - 1] > ratings[i])
        {
            payouts[i - 1] = std::max(payouts[i - 1], 5 + payouts[i]);
        }
    }

    std::ofstream output("output.txt");
    output << std::reduce(payouts.begin(), payouts.end()) * 100;

    return 0;
}