#include <fstream>
#include <string>

#define HAS_DIGIT_OFFSET 0
#define HAS_UPPERCASE_OFFSET 1
#define HAS_LOWERCASE_OFFSET 2
#define IS_VALID 0b111

#define MIN_NAME_SIZE 8

bool isDigit(char ch)
{
    return ch >= '0' && ch <= '9';
}

bool isUpper(char ch)
{
    return ch >= 'A' && ch <= 'Z';
}

bool isLower(char ch)
{
    return ch >= 'a' && ch <= 'z';
}

int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    std::string username;
    input >> username;

    if(username.size() < MIN_NAME_SIZE)
    {
        output << "NO";
        return 0;
    }

    int checkStatus = 0;

    for(size_t i = 0; i < username.size(); ++i)
    {
        checkStatus |= isDigit(username[i]) << HAS_DIGIT_OFFSET;
        checkStatus |= isUpper(username[i]) << HAS_UPPERCASE_OFFSET;
        checkStatus |= isLower(username[i]) << HAS_LOWERCASE_OFFSET;
    }

    std::string res = checkStatus == IS_VALID ? "YES" : "NO";
    output << res;

    return 0;
}