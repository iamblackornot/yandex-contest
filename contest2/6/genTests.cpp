#include <fstream>
#include <vector>
#include <random>

int main()
{
    std::ofstream outputFile("input.txt");

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> sizeGen(1, 100); // define the range
    std::uniform_int_distribution<> elGen(-100, 100); // define the range

    for(size_t testNum = 0; testNum < 1000; ++testNum)
    {
        size_t cols = sizeGen(gen);
        size_t rows = sizeGen(gen);

        std::uniform_int_distribution<> kGen(1, cols * rows);
        size_t k = kGen(gen);

        outputFile << k << std::endl << cols << " " << rows << std::endl;

        for(size_t row = 0; row < rows; ++row)
        {
            for(size_t col = 0; col < cols; ++col)
            {
                outputFile << elGen(gen) << " "; 
            }

            outputFile << std::endl;
        }
    }
    return 0;
}