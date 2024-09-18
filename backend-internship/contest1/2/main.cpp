#include <fstream>
#include <string>
#include <list>

typedef std::string::const_iterator LogsIter;

std::string parseAction(LogsIter& logsIt)
{
    std::string action;
    std::advance(logsIt, 1);

    while(*logsIt != '>') {
        action.push_back(*logsIt);
        std::advance(logsIt, 1);
    }

    std::advance(logsIt, 1);
    return action;
}

typedef std::list<char>::iterator InputBufIter;

class UserInput
{
public:
    UserInput();
    void AddKey(char key);
    void Delete();
    void Backspace();
    void Left();
    void Right();
    bool operator==(std::string const rhs) const;
private:
    std::list<char> buffer;
    InputBufIter cursorIt;
};

UserInput::UserInput() : cursorIt(buffer.end()) {}

void UserInput::AddKey(char key)
{
    buffer.insert(cursorIt, key);
}

void UserInput::Delete()
{
    if(cursorIt != buffer.end())
    {
        cursorIt = buffer.erase(cursorIt);
    }
}

void UserInput::Backspace()
{
    if(cursorIt != buffer.begin())
    {
        std::advance(cursorIt, -1);
        cursorIt = buffer.erase(cursorIt);
    }
}

void UserInput::Left()
{
    if(cursorIt != buffer.begin())
    {
        std::advance(cursorIt, -1);
    }
}

void UserInput::Right()
{
    if(cursorIt != buffer.end())
    {
        std::advance(cursorIt, 1);
    }
}

bool UserInput::operator==(std::string const rhs) const
{
    if(buffer.size() != rhs.size()) { return false; }

    auto itBuffer = buffer.begin();
    auto itString = rhs.begin();
    
    while(itBuffer != buffer.end())
    {
        if(*itBuffer != *itString) { return false; }
        std::advance(itBuffer, 1);
        std::advance(itString, 1);
    }

    return true;
}

int main() 
{
    std::string check;
    std::string logs;
    std::ifstream input("input.txt");

    input >> check;
    input >> logs;

    UserInput inputBuffer;
    auto logsIt = logs.cbegin();

    while(logsIt != logs.end()) 
    {
        if(*logsIt != '<') 
        {
            inputBuffer.AddKey(*logsIt);
            std::advance(logsIt, 1);
            continue;
        }

        std::string action = parseAction(logsIt);

        if(action == "delete") 
        {
            inputBuffer.Delete();
        } 
        else if(action == "bspace") 
        {
            inputBuffer.Backspace();
        }
        else if(action == "left") 
        {
            inputBuffer.Left();
        } 
        else if(action == "right") 
        {
            inputBuffer.Right();
        }
    }

    std::string res = inputBuffer == check ? "YES" : "NO";
    std::ofstream output("output.txt");

    output << res;
    
    return 0;
}