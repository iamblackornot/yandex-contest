#include <fstream>
#include <vector>
#include <deque>

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int size;
    int wndSize;

    input >> size;
    input >> wndSize;

    std::deque<int> deque;
    std::vector<int> numbers(size);

    for(int i = 0; i < wndSize - 1; ++i)
    {
        input >> numbers[i];

        while(!deque.empty()
            && numbers[deque.back()] >= numbers[i])
        {
            deque.pop_back();
        }

        deque.push_back(i);
    }

    for(int i = wndSize - 1; i < size; ++i)
    {
        input >> numbers[i];

        while(!deque.empty()
            && numbers[deque.back()] >= numbers[i])
        {
            deque.pop_back();
        }

        deque.push_back(i);

        if(deque.front() == i - wndSize) { deque.pop_front(); }

        output << numbers[deque.front()] << std::endl;
    }

    return 0;
}

