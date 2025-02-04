#include <fstream>
#include <stack>
#include <string>

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    std::string str;
    std::getline(input, str);

    std::stack<int> stack;

    for(char const ch : str)
    {
        if(std::isspace(ch)) { continue; }
        if(std::isdigit(ch)) { stack.push(ch - '0'); }
        else
        {
            int rhs = stack.top();
            stack.pop();
            int lhs = stack.top();
            stack.pop();

            if      (ch == '+') { stack.push(lhs + rhs); }
            else if (ch == '-') { stack.push(lhs - rhs); }
            else                { stack.push(lhs * rhs); }
        }
    }

    output << stack.top();
    
    return 0;
}

