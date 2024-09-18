#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <iomanip>

class Stats
{
    public:
        void Add(double value)
        {
            double fraction = 1 / value;

            if(partialSum.size() > 0)
            {
                partialSum.push_back(
                    fraction + partialSum[partialSum.size() - 1]);
            }
            else
            {
                partialSum.push_back(fraction);
            } 
        }
        double GetHarmonicAverage(int start, int end)
        {
            double denominator = partialSum[end];

            if(start > 0)
            {
                denominator -= partialSum[start - 1];
            }

            return (end - start + 1) / denominator;
        }
    private:
        std::vector<double> partialSum;
};



int main()
{
    std::ifstream inputFile("input.txt");

    int n;
    inputFile >> n;

    Stats stats;

    for(int i = 0; i < n; ++i)
    {
        double value;
        inputFile >> value;

        stats.Add(value);
    }

    std::ofstream outputFile("output.txt");

    inputFile >> n;

    outputFile << std::setprecision(6) << std::fixed;

    for(int i = 0; i < n; ++i)
    {
        int start;
        int end;

        inputFile >> start >> end;

        outputFile << stats.GetHarmonicAverage(start, end) << std::endl;
    }

    return 0;
}

