#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ranges>
#include <algorithm>

std::string NumToString(int value, int totalLength) 
{
    std::stringstream ss;
    ss << std::setw(totalLength) << std::setfill('0') << value;

    return ss.str();
}

int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int maxHours;
    int maxMinutes;

    input >> maxHours;
    input >> maxMinutes;

    int min = std::min(maxHours, maxMinutes);
    int max = std::max(maxHours, maxMinutes);

    std::string maxString = std::to_string(max - 1);
    int totalLength = std::to_string(max - 1).size();
    int count = 0;

    for(size_t i = 0; i < min; ++i) 
    {
        std::string numString = NumToString(i, totalLength);
        std::reverse(numString.begin(), numString.end());

        if(numString <= maxString) ++count;
    }

    output << count;
    
    return 0;
}