#include <fstream>

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int size;
    input >> size;

    int sum = 0;

    for(int i = 0; i < size; ++i)
    {
        if(i > 0) { output << " "; }

        int value;
        input >> value;

        sum += value;
        output << sum;
    }

    return 0;
}