#include <vector>
#include <memory>
#include <fstream>
#include <cmath>
#include <numeric>
#include <iostream>

int main()
{
    std::ifstream inputFile("input.txt");

    size_t n;
    inputFile >> n;

    std::vector<int> interestingBooks;
    std::vector<int> boringBooks;

    for(size_t i = 0; i < n; ++i)
    {
        int bookValue;
        inputFile >> bookValue;

        if(bookValue < 0)
        {
            boringBooks.push_back(bookValue);
        }
        else
        {
            interestingBooks.push_back(bookValue);
        }
    }

    std::ofstream outputFile("output.txt"); 
    outputFile.precision(20);

    size_t overallInterestingReadTime = 0;

    for(auto const& book : interestingBooks)
    {
        overallInterestingReadTime += book;
    }

    if(boringBooks.size() == 0)
    {
        outputFile << overallInterestingReadTime;
        return 0;
    }

    size_t overallBoringReadTime = 0;

    for(auto const& val : boringBooks)
    {
        overallBoringReadTime += abs(val);
    }

    long double averageBoringReadTime = overallBoringReadTime / (long double)boringBooks.size();

    if(interestingBooks.size() == 0)
    {
        outputFile << averageBoringReadTime;
        return 0;
    }

    long double evOverall = averageBoringReadTime; 
    long double probabilityMultiplier = 1 / (long double)(n);

    evOverall += probabilityMultiplier * overallInterestingReadTime;
    size_t selectionWays = 1;

    for(size_t i = 1; i < interestingBooks.size(); ++i)
    {
        probabilityMultiplier *= 1 / (long double)(n - i);
        selectionWays *= interestingBooks.size() - i;

        evOverall += probabilityMultiplier * selectionWays * overallInterestingReadTime;
    }

    outputFile << evOverall;

    return 0;
}