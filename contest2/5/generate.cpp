#include <fstream>
#include <cstdlib>
#include <ctime>

int main()
{
    std::ofstream outputFile("input.txt");

    srand(time(nullptr));

    for(size_t i = 0; i < 1000; ++i)
    {
        int randomN = rand() % 14 + 1;
        outputFile << randomN;

        for(size_t j = 0; j < randomN; j++)
        {
            outputFile << " " << rand() % 201 - 100;
        }

        outputFile << std::endl;
    }
}