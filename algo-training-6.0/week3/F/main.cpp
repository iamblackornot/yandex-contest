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

    int size;
    input >> size;

    std::string prioStr;
    input >> prioStr;

    std::string str;
    input >> str;

    std::stack<char> stack;
    int openingCount = 0;

    for(char const ch : str)
    {
        if(isOpening(ch)) 
        { 
            stack.push(ch);
            ++openingCount;
        }
        else { stack.pop(); }
    }

    int openingLeft = size / 2 - openingCount;
    output << str;

    while(!stack.empty() || openingLeft > 0)
    {
        for(int i = 0; i < prioStr.size(); ++i)
        {
            if(!isOpening(prioStr[i]))
            {
                if(!stack.empty() && openToClose.at(stack.top()) == prioStr[i]) 
                { 
                    stack.pop();
                    output << prioStr[i];
                    break;
                }

            }
            else if(openingLeft > 0)
            {
                output << prioStr[i];
                stack.push(prioStr[i]);
                --openingLeft;
                break;
            }
        }
    }

    return 0;
}

// 10
// ][()
// ([](

// ([]([][])) 