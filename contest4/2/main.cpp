#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>

int ManhattanDistanceFromCenter(int x, int y) 
{
    return abs(x) + abs(y);
}

bool CanTakeOrderFromCentralPart(
    int distanceFromCenter,
    int orderClaimDistance,
    int centralPartRadius)
{
    return distanceFromCenter - centralPartRadius <= orderClaimDistance; 
}

struct CarParseResult
{
    int minimalCarDistance;
    std::vector<int> carDistances;
};

CarParseResult ParseAndFilterOutCars(std::istream& input, int centralPartRadius, int orderClaimDistance)
{
    CarParseResult res;
    res.minimalCarDistance = (std::numeric_limits<int>::max)();

    int carNum;
    input >> carNum;

    for(size_t i = 0; i < carNum; ++i)
    {
        int x, y;

        input >> x;
        input >> y;

        int distance = ManhattanDistanceFromCenter(x, y);

        if(CanTakeOrderFromCentralPart(distance, orderClaimDistance, centralPartRadius))
        {
            res.minimalCarDistance = std::min(distance, res.minimalCarDistance);
            res.carDistances.push_back(distance);
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

    int centralPartRadius, orderClaimDistance;

    input >> centralPartRadius;
    input >> orderClaimDistance;

    CarParseResult firstCompany = ParseAndFilterOutCars(input, centralPartRadius, orderClaimDistance);
    CarParseResult secondCompany = ParseAndFilterOutCars(input, centralPartRadius, orderClaimDistance);

    std::ofstream output("output.txt");

    if(firstCompany.carDistances.empty() && secondCompany.carDistances.empty())
    {
        PrintDraw(output);
    }
    else if(firstCompany.minimalCarDistance < secondCompany.minimalCarDistance)
    {
        PrintFirstWin(output, std::ranges::count_if(firstCompany.carDistances, 
            [=](int const& el) 
            { 
                return el < secondCompany.minimalCarDistance;
            }
        ));
    }
    else if(firstCompany.minimalCarDistance > secondCompany.minimalCarDistance)
    {
        PrintSecondWin(output, std::ranges::count_if(secondCompany.carDistances,
            [=](int const& el) 
            { 
                return el < firstCompany.minimalCarDistance;
            }
        ));
    }
    else
    {
        PrintDraw(output);
    }

    return 0;
}