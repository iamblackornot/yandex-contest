#include <fstream>
#include <stack>
#include <vector>

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int size;
    input >> size;

    std::stack<int> stack;
    std::vector<int> cost(size);
    std::vector<int> res(size, -1);

    for(int i = 0; i < size; ++i)
    {
        input >> cost[i];

        while(!stack.empty() 
            && cost[stack.top()] > cost[i])
        {
            res[stack.top()] = i;
            stack.pop();
        }

        stack.push(i);
    }

    for(int i = 0; i < size; ++i)
    {
        if(i != 0) { output << " "; }

        output << res[i];
    }

    return 0;
}

