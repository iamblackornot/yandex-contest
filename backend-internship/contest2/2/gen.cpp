#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <cmath>
#include <ranges>
#include <random>

int main()
{
    std::ofstream outputFile("input.txt");

    // std::random_device dev;
    // std::mt19937 rng(dev());
    // std::uniform_int_distribution<std::mt19937::result_type> distPrice(1, 1e6);
    // std::uniform_int_distribution<std::mt19937::result_type> distN(1, 1e5);
    // std::uniform_int_distribution<std::mt19937::result_type> distK(1, 4);
    
    // int n = distN(rng);
    // outputFile << n << " " << distK(rng) << "\n";

    // for(int i = 0; i < n; ++i)
    // {
    //     outputFile << distPrice(rng) << " ";
    // }
    
    int64_t n = 1e5;
    int k = 1e6;
    int64_t price = n;

    outputFile << n << " " << k << "\n";
    int64_t sum = 0;

    for(int i = 0; i < n; ++i)
    {
        outputFile << price << " ";
        sum += price;
        --price;
    }

    std::cout << sum;
    std::cout << "\n" << n * (n + 1) / 2;

    return 0;
}