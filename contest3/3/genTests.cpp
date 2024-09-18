#include <fstream>
#include <vector>
#include <random>

int main()
{
    std::ofstream outputFile("input.txt");

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> sizeGen(2, 300); // define the range
    std::uniform_int_distribution<> squareGen(1, 400); // define the range

    for(size_t testNum = 0; testNum < 1000; ++testNum)
    {
        int n = sizeGen(gen);
        int s = squareGen(gen);

        std::uniform_int_distribution<> rangeGen(0, 2000); // define the range
        int maxCoord = rangeGen(gen);
        int minCoord = std::max(maxCoord - 20, 0);
        std::uniform_int_distribution<> coordGen(minCoord, maxCoord); // define the range

        outputFile << n << " " << s << std::endl;

        for(size_t i = 0; i < n; ++i)
        {
            int x = coordGen(gen);
            int y = coordGen(gen);

            outputFile << x << " " << y << std::endl;
        }

        outputFile << std::endl;
    }
    
    return 0;
}