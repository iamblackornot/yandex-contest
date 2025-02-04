#include <fstream>
#include <stack>
#include <string>
#include <unordered_map>

bool isOpening(char ch)
{
    return ch == '(' || ch == '{' || ch == '[';
}

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");
    std::unordered_map<char, char> openToClose { { '(', ')' }, { '{', '}' }, { '[', ']' } };

    std::string str;
    input >> str;

    std::stack<char> stack;

    for(char const ch : str)
    {
        if(isOpening(ch)) { stack.push(ch); }
        else if(stack.empty() || ch != openToClose.at(stack.top()))
        {
            output << "no";
            return 0;
        }
        else { stack.pop(); }
    }

    output << (stack.empty() ? "yes" : "no");
    return 0;
}

