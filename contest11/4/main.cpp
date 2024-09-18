#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <ranges>
#include <string>
#include <format>

void Decrement(std::string& value)
{
    if(value.empty()) return;

    for(size_t i = 1; i < value.size(); ++i)
    {
        size_t index = value.size() - i;
        char ch = value[index];

        if(ch > '0')
        {
            value[index] = --ch;
            return;
        }

        value[index] = '9';
    }

    char ch = value[0];
    value[0] = ch > '0' ? --ch : '9';

    return;
}

void Increment(std::string& value)
{
    if(value.empty()) return;

    for(size_t i = 1; i < value.size(); ++i)
    {
        size_t index = value.size() - i;
        char ch = value[index];

        if(ch < '9')
        {
            value[index] = ++ch;
            return;
        }

        value[index] = '0';
    }

    char ch = value[0];
    value[0] = ch < '9' ? ++ch : '0';

    return;
}

bool LessOrEqualReverse(std::string const& str, std::string const& revStr)
{
    size_t length = revStr.length();

    for(size_t i = 0; i < length; ++i)
    {
        if(revStr[length - i - 1] < str[i]) return true;
        if(revStr[length - i - 1] > str[i]) return false;
    }

    return true;
}

std::string AddLeadingZeros(std::string str, size_t totalLength)
{
    if(str.length() >= totalLength) return str;

    size_t diff = totalLength - str.length();
    std::string res(totalLength, '0');
    std::ranges::copy(str, std::next(res.begin(), diff));
    return res;
}


int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    std::string maxHours;
    std::string maxMinutes;

    input >> maxHours;
    input >> maxMinutes;

    Decrement(maxHours);
    Decrement(maxMinutes);

    size_t totalLength = std::max(maxHours.size(), maxMinutes.size());

    maxHours = AddLeadingZeros(maxHours, totalLength);
    maxMinutes = AddLeadingZeros(maxMinutes, totalLength);

    std::string minString = std::min(maxHours, maxMinutes);
    std::string maxString = std::max(maxHours, maxMinutes);

    //int count = 0;


    std::string itString(totalLength, '0');
    int count = 0;

    while(itString <= minString)
    {
        if(LessOrEqualReverse(maxString, itString)) 
        { 
            ++count; 
            output << std::format("{} - {}\n", count, itString);
        }
        Increment(itString);
    }

    output << count;

    return 0;
}