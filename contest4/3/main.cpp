#include <fstream>
#include <vector>
#include <limits>
#include <unordered_map>

typedef std::unordered_map<int, int> ModuloDayMap;

uint64_t AttendanceCount(ModuloDayMap const& moduloToDays, uint64_t day, int interval) 
{
    uint64_t count = 0;

    for(auto const& [modulo, startDay] : moduloToDays) 
    {
        if(startDay <= day)
        {
            count += (day - startDay) / interval + 1;
        }
    }

    return count;
}

int main() 
{
    std::ifstream input("input.txt");

    uint64_t subjectNum, interval, targetAttendance;
    input >> subjectNum;
    input >> interval;
    input >> targetAttendance;

    std::vector<int> startDays;
    startDays.reserve(subjectNum);

    int firstDay = (std::numeric_limits<int>::max)();

    for(size_t i = 0; i < subjectNum; ++i)
    {
        int value;
        input >> value;

        startDays.push_back(value); 
        firstDay = std::min(value, firstDay);
    }

    ModuloDayMap moduloToDay;

    for(size_t i = 0; i < subjectNum; ++i)
    {
        int modulo = (startDays.at(i) - firstDay) % interval;

        if(!moduloToDay.contains(modulo) || moduloToDay.at(modulo) >= startDays.at(i)) 
        {
            moduloToDay[modulo] = startDays.at(i);
        }   
    }

    uint64_t rangeStart = firstDay;
    uint64_t rangeEnd = rangeStart + (targetAttendance - 1) * interval;
    uint64_t res = 0;

    while(rangeStart <= rangeEnd)
    {
        uint64_t middleDay = rangeStart + (rangeEnd - rangeStart) / 2;
        uint64_t currDayAttendance = AttendanceCount(moduloToDay, middleDay, interval);

        if(currDayAttendance < targetAttendance)
        {
            rangeStart = middleDay + 1;
        }
        else if(currDayAttendance > targetAttendance)
        {
            rangeEnd = middleDay - 1;
        } 
        else
        {
            uint64_t dayBeforeAttendance = AttendanceCount(moduloToDay, middleDay - 1, interval);

            if(dayBeforeAttendance == targetAttendance - 1)
            {
                res = middleDay;
                break;
            }

            rangeStart = std::max(middleDay - interval, rangeStart);
            rangeEnd = middleDay - 1;
        }
    }

    std::ofstream output("output.txt");
    output << res;

    return 0;
}