#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <ranges>

#define TOWERS_PER_COMPANY 8

double DistanceFromCenter(int x, int y) 
{
    return sqrt(x * x + y * y);
}

bool CoversCityArea(
    double distanceFromCenter,
    int towerCoverRadius,
    int cityAreaRadius)
{
    return cityAreaRadius + towerCoverRadius >= distanceFromCenter ; 
}

struct TowerParseResult
{
    double minTowerDistanceFromCenter;
    std::vector<double> towerDistances;
};

TowerParseResult ParseAndFilterOutTowers(std::istream& input, int cityAreaRadius, int towerCoverRadius)
{
    TowerParseResult res;
    res.minTowerDistanceFromCenter = (std::numeric_limits<int>::max)();

    for(size_t i = 0; i < TOWERS_PER_COMPANY; ++i)
    {
        int x, y;

        input >> x;
        input >> y;

        double distance = DistanceFromCenter(x, y);

        if(CoversCityArea(distance, towerCoverRadius, cityAreaRadius))
        {
            res.minTowerDistanceFromCenter = std::min(distance, res.minTowerDistanceFromCenter);
            res.towerDistances.push_back(distance);
        }
    }

    return res;
}

void PrintResult(std::ostream& output, int winner, int credibility)
{
    output << winner << std::endl << credibility;
}

void PrintDraw(std::ostream& output)
{
    PrintResult(output, 0, 0);
}

void PrintFirstWin(std::ostream& output, int credibility)
{
    PrintResult(output, 1, credibility);
}

void PrintSecondWin(std::ostream& output, int credibility)
{
    PrintResult(output, 2, credibility);
}

int main() 
{
    std::ifstream input("input.txt");

    int cityAreaRadius, towerCoverRadius;

    input >> cityAreaRadius;
    input >> towerCoverRadius;

    TowerParseResult firstCompany = ParseAndFilterOutTowers(input, cityAreaRadius, towerCoverRadius);
    TowerParseResult secondCompany = ParseAndFilterOutTowers(input, cityAreaRadius, towerCoverRadius);

    std::ofstream output("output.txt");

    if(firstCompany.towerDistances.empty() && secondCompany.towerDistances.empty())
    {
        PrintDraw(output);
    }
    else if(firstCompany.minTowerDistanceFromCenter < secondCompany.minTowerDistanceFromCenter)
    {
        PrintFirstWin(output, std::ranges::count_if(firstCompany.towerDistances, 
            [=](double const& el) 
            { 
                return el < secondCompany.minTowerDistanceFromCenter;
            }
        ));
    }
    else if(firstCompany.minTowerDistanceFromCenter > secondCompany.minTowerDistanceFromCenter)
    {
        PrintSecondWin(output, std::ranges::count_if(secondCompany.towerDistances,
            [=](double const& el) 
            { 
                return el < firstCompany.minTowerDistanceFromCenter;
            }
        ));
    }
    else
    {
        PrintDraw(output);
    }

    return 0;
}