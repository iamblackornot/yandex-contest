#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <limits>
#include <string>

int main() 
{
    std::ofstream output("input.txt");

    int count = 100'001;

    output << count << "\n";

    for(size_t i = 1; i < count; ++i)
    {
        output << "worker" << i << " " << 1 << "\n";
    }

    output << "worker" << count << " " << count << "\n";

    return 0;
}