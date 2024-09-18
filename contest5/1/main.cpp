#include <fstream>
#include <stack>

class MaxStack
{
public:
    void push(int val);
    void pop();
    int max() const;
private:
    std::stack<int> values;
    std::stack<int> maxValues;
};

void MaxStack::push(int val)
{
    if(maxValues.empty())
    {
        maxValues.push(val);
    }
    else
    {
        maxValues.push(std::max(val, maxValues.top()));
    }

    values.push(val);
}

void MaxStack::pop()
{
    values.pop();
    maxValues.pop();
}

int MaxStack::max() const
{
    if(maxValues.empty())
    {
        return 0;
    }

    return maxValues.top();
}

int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int queryCount;
    input >> queryCount;

    MaxStack stack;

    for(size_t i = 0; i < queryCount; i++) 
    {
        std::string command;
        input >> command;

        if(command == "max") 
        {
            output << stack.max() << std::endl;
        } 
        else if(command == "push") 
        {
            int value;
            input >> value;

            stack.push(value);
        } 
        else if(command == "pop") 
        {
            stack.pop();
        }
    }

    return 0;
}