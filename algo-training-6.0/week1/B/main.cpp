#include <fstream>

int main() 
{    
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int blueShirts;
    int redShirts;
    int blueSocks;
    int redSocks;

    input >> blueShirts;
    input >> redShirts;
    input >> blueSocks;
    input >> redSocks;

    if (blueShirts == 0 || blueSocks == 0)
    {
        output << blueShirts + 1 << " " << blueSocks + 1;
    }
    else if (redShirts == 0 || redSocks == 0)
    {
        output << redShirts + 1 << " " << redSocks + 1;
    }
    else
    {
        int blueSum = blueShirts + blueSocks + 2;
        int redSum = redShirts + redSocks + 2;

        int basicSum = std::min(blueSum, redSum);
        int anySocksSum = std::max(blueShirts, redShirts) + 2;
        int anyShirtSum = std::max(blueSocks, redSocks) + 2;

        int minSum = std::min(basicSum, std::min(anySocksSum, anyShirtSum));

        if(minSum == basicSum)
        {
            if(blueSum < redSum)
                output << blueShirts + 1 << " " << blueSocks + 1; 
            else
                output << redShirts + 1 << " " << redSocks + 1;
        }
        else if(minSum == anySocksSum)
        {
            output << std::max(blueShirts, redShirts) + 1 << " " << 1;
        }
        else
        {
            output << 1 << " " << std::max(blueSocks, redSocks) + 1;
        }
    }

    return 0;
}

// 1000000000 1000000000 1000000000 1000000000
// 1 0 1 1000000000
// 1 1000000000 1 0
// 0 1000000000 0 1000000000
// 6 2 7 3
// 2 6 3 7
// 1 3 100000 2
// 1 1 1 1