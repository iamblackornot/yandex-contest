#include <fstream>
#include <string>

constexpr char EOL = 10;
using CharBufIter = std::istreambuf_iterator<char>;
bool ScanExpression(CharBufIter& it);



bool IsEOLorEOS(CharBufIter& it)
{
    return it == CharBufIter() || *it == EOL;
}

void SkipWhitespaces(CharBufIter& it)
{
    while(!IsEOLorEOS(it) && std::isspace(*it)) 
    {
         ++it;
    }
}

bool IsSpecialCharacter(char ch)
{
    return 
        ch == '{' ||
        ch == '}' ||
        ch == '(' ||
        ch == ')' ||
        ch == '[' ||
        ch == ']';
}

bool IsCharacter(CharBufIter& it, char ch)
{
    if(IsEOLorEOS(it) || *it != ch) { return false; }

    return true;
}

bool ScanCharacter(CharBufIter& it, char ch)
{
    SkipWhitespaces(it);  
    
    if(IsEOLorEOS(it)) { return false; }
    if(*it != ch) { return false; }

    ++it;

    return true;
}

bool ScanLiteral(CharBufIter& it)
{
    SkipWhitespaces(it);

    if(IsEOLorEOS(it)) { return false; }

    size_t length = 0;

    while(!IsEOLorEOS(it) && !IsSpecialCharacter(*it) && !std::isspace(*it))
    {
        ++length;
        ++it;
    }

    return length > 0;
}

bool ScanTag(CharBufIter& it)
{
    SkipWhitespaces(it);    

    if(!ScanCharacter(it, '{')) { return false; }
    //while(ScanLiteral(it)) {};
    if(!ScanLiteral(it)) { return false; }
    if(!ScanCharacter(it, '}')) { return false; }

    return true;
}

bool ScanPredicate(CharBufIter& it)
{
    SkipWhitespaces(it);    

    if(!ScanCharacter(it, '[')) { return false; }
    //while(ScanLiteral(it)) {};
    if(!ScanLiteral(it)) { return false; }
    if(!ScanCharacter(it, ']')) { return false; }

    return true;
}

bool ScanArgument(CharBufIter& it)
{
    if(ScanCharacter(it, '('))
    {
        return ScanExpression(it) && ScanCharacter(it, ')');
    }

    return ScanTag(it) || ScanPredicate(it);
}

bool ScanOperator(CharBufIter& it)
{
    SkipWhitespaces(it);    

    if(!ScanLiteral(it)) { return false; }
    while(ScanLiteral(it)) {};

    return true;
}

bool ScanExpression(CharBufIter& it)
{
    bool startsWithArgument = ScanArgument(it);
    bool atLeastOneOperator = false;

    while(ScanOperator(it))
    {
        if(!ScanArgument(it)) { return false; }
        atLeastOneOperator = true;
    }

    SkipWhitespaces(it);

    return startsWithArgument || atLeastOneOperator;
}

bool SkipToNewLine(CharBufIter& it)
{
    while(it != CharBufIter{})
    {
        if(*it == EOL)
        {
            ++it;
            return true;
        }

        ++it;    
    }

    return false;
}

int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    CharBufIter iter{ input };

    while(SkipToNewLine(iter))
    {
        if(ScanExpression(iter) && IsEOLorEOS(iter))
        {
            output << "valid";
        }
        else
        {
            output << "invalid";
        }

        output << std::endl;
    }

    return 0;
}