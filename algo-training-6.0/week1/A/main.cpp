#include <fstream>

int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int botX;
    int botY;
    int topX;
    int topY;

    input >> botX;
    input >> botY;
    input >> topX;
    input >> topY;

    int x;
    int y;

    input >> x;
    input >> y;

    if      (x < botX)
    {
        if      (y < botY)  { output << "SW"; }
        else if (y < topY)  { output <<  "W"; }
        else                { output << "NW"; }
    }
    else if (x < topX)
    {
        if      (y < botY)  { output << "S"; }
        else                { output << "N"; }
    }
    else
    {
        if      (y < botY)  { output << "SE"; }
        else if (y < topY)  { output <<  "E"; }
        else                { output << "NE"; }
    }

    return 0;
}