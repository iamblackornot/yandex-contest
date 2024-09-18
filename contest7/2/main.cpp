#include <fstream>
#include <unordered_map>
#include <deque>

constexpr int ONE_SECOND_IN_MILLISECONDS = 1000;
using timestamp = unsigned long long;

int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    std::unordered_map<std::string, std::deque<timestamp>> tokens;

    int rateLimit, requestCount;
    input >> rateLimit;
    input >> requestCount;

    for(int i = 0; i < requestCount; ++i) 
    {
        timestamp time;
        std::string token;

        input >> time;
        input >> token;

        std::deque<timestamp>& timestamps = tokens[token];

        while(!timestamps.empty() && time - timestamps[0] >= ONE_SECOND_IN_MILLISECONDS) 
        {
            timestamps.pop_front();
        }
        
        if(timestamps.size() < rateLimit) 
        {
            timestamps.push_back(time);
            output << time << " " << token << std::endl;
        }
    }

    return 0;
}