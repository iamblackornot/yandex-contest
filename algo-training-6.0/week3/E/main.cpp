#include <fstream>
#include <stack>
#include <string>
#include <vector>

enum class Category : int
{
    VALUE = 0,
    OPERATOR = 1,
    BRACKET = 2,
};

enum class Operator : int
{
    UNARY_SUB = 0,
    SUM = 1,
    SUB = 2,
    MULT = 3,
};

enum class Bracket : int
{
    OPENING = 0,
    CLOSING = 1,
};

struct Token 
{
    Category category;
    int value;
};

typedef std::vector<Token> TokenList;

class Tokenizer
{
public:
    bool tokenizeInfixString(std::string_view expression, TokenList& tokens) 
    {
        int offset = 0;
        return parseExpression(expression, tokens, offset, '\0');
    }
private:
    void skipWhitespaces(std::string_view expression, int& offset) 
    {
        char currCh = expression[offset];

        while (currCh && std::isspace(currCh)) 
        {
            ++offset;
            currCh = expression[offset];
        }
    }

    bool parseUnarySub(std::string_view expression, TokenList& tokens, int& offset) 
    {
        if (expression[offset] != '-') {
            return false;
        }

        tokens.push_back( Token{ Category::OPERATOR, (int)Operator::UNARY_SUB } );
        ++offset;

        return true;
    }

    int parseConstant(std::string_view expression, TokenList& tokens, int& offset) 
    {
        bool negative = false;
        int result = 0;
 
        if(!std::isdigit(expression[offset])) { return false; }

        while (std::isdigit(expression[offset]))
        {
            int digit = expression[offset] - '0';
            result *= 10;
            result += digit;
            ++offset;
        }
    
        tokens.push_back( Token{ Category::VALUE, result } );

        return true;
    }

    bool parseOpeningBracket(std::string_view expression, TokenList& tokens, int& offset) 
    {
        if (expression[offset] != '(') {
            return false;
        }

        tokens.push_back( Token{ Category::BRACKET, (int)Bracket::OPENING } );
        offset += 1;

        return true;
    }

    int parseClosingBracket(std::string_view expression, TokenList& tokens, int& offset) 
    {
        if (expression[offset] != ')') {
            return false;
        }

        tokens.push_back( Token{ Category::BRACKET, (int)Bracket::CLOSING } );
        offset += 1;

        return true;
    }

    int parseExpressionInBrackets(std::string_view expression, TokenList& tokens, int& offset) 
    {
        int newOffset = offset;

        if (!parseOpeningBracket(expression, tokens, newOffset)) 
        {
            return false;
        }

        if (!parseExpression(expression, tokens, newOffset, ')')) 
        {
            return false;
        }

        skipWhitespaces(expression, newOffset);

        if (!parseClosingBracket(expression, tokens, newOffset)) 
        {
            return false;
        }

        offset = newOffset;

        return true;
    }

    int getOperatorId(const char ch) {
        if (ch == '+') {
            return (int)Operator::SUM;
        }

        if (ch == '-') {
            return (int)Operator::SUB;
        }

        if (ch == '*') {
            return (int)Operator::MULT;
        }

        return -1;
    }

    bool parseOperand(std::string_view expression, TokenList& tokens, int& offset) 
    {
        if (parseConstant(expression, tokens, offset))
        {
            return true;
        }

        if (parseExpressionInBrackets(expression, tokens, offset))
        {
            return true;
        }

        return false;
    }

    bool parseOperator(std::string_view expression, TokenList& tokens, int& offset) 
    {
        int operatorId = getOperatorId(expression[offset]);

        if (operatorId < 0) {
            return false;
        }

        tokens.push_back({ Category::OPERATOR, operatorId });
        ++offset;

        return true;
    }

    bool parseExpression(std::string_view expression, TokenList& tokens, int& offset, char endChar) 
    {
        skipWhitespaces(expression, offset);

        parseUnarySub(expression, tokens, offset);
        skipWhitespaces(expression, offset);

        if (!parseOperand(expression, tokens, offset)) 
        {
            return false;
        }

        skipWhitespaces(expression, offset);

        while (expression[offset] != endChar && offset < expression.size()) 
        {
            if (!parseOperator(expression, tokens, offset)) 
            {
                return false;
            }

            skipWhitespaces(expression, offset);

            if (expression[offset] == endChar || offset >= expression.size()) 
            {
                return false;
            }

            if (!parseOperand(expression, tokens, offset)) 
            {
                return false;
            }

            skipWhitespaces(expression, offset);
        }

        return true;
    }
};

class PostfixConverter
{
public:
    TokenList ToPostfix(TokenList const& infix) 
    {
        TokenList postfix;
        std::stack<Token> stack;

        for(Token token : infix)
        {
            if (token.category == Category::VALUE) 
            {
                postfix.push_back(token);
            }
            else if (token.category == Category::OPERATOR) 
            {
                popOperatorsWithHigherPrio(token, stack, postfix);
                stack.push(token);
            } 
            else if (token.category == Category::BRACKET) 
            {
                if (token.value == (int)Bracket::OPENING) 
                {
                    stack.push(token);
                } 
                else 
                {
                    popStackUpToBracket(stack, postfix);
                }
            }
        }

        popRemainingStack(stack, postfix);

        return postfix;
    }
    
private:
    void popOperatorsWithHigherPrio(Token currToken, std::stack<Token>& stack, TokenList& postfix) 
    {
        while (!stack.empty() && stack.top().category == Category::OPERATOR) 
        {
            Token topToken = stack.top();

            if(topToken.category != Category::OPERATOR) { return; }

            int topPrio = getOperatorPriority(topToken.value);
            int currPrio = getOperatorPriority(currToken.value);

            if (topPrio < currPrio) { return; }

            postfix.push_back(topToken);
            stack.pop();
        }
    }

    void popStackUpToBracket(std::stack<Token>& stack, TokenList& postfix) 
    {
        while (!stack.empty() 
            && stack.top().category != Category::BRACKET 
            && stack.top().value != (int)Bracket::OPENING) 
        {
            postfix.push_back(stack.top());
            stack.pop();
        }

        stack.pop();
    }

    void popRemainingStack(std::stack<Token>& stack, TokenList& postfix) 
    {
        while (!stack.empty()) 
        {
            postfix.push_back(stack.top());
            stack.pop();
        }
    }

    int getOperatorPriority(int type) {
        if (type == (int)Operator::MULT) {
            return 1;
        }

        return 0;
    }
};


int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    std::string str;
    std::getline(input, str);

    Tokenizer tokenizer;
    PostfixConverter converter;
    TokenList tokens;
    
    if(!tokenizer.tokenizeInfixString(str, tokens))
    {
        output << "WRONG";
        return 0;
    }

    tokens = converter.ToPostfix(tokens);
    std::stack<int> stack;

    for(auto token : tokens)
    {
        if(token.category == Category::VALUE) { stack.push(token.value); }
        else
        {
            int rhs = stack.top();
            stack.pop();

            if (token.value == (int)Operator::UNARY_SUB)
            { 
                stack.push(- rhs);
                continue;
            }

            int lhs = stack.top();
            stack.pop();

            if      (token.value == (int)Operator::SUM) { stack.push(lhs + rhs); }
            else if (token.value == (int)Operator::SUB) { stack.push(lhs - rhs); }
            else                                        { stack.push(lhs * rhs); }
        }
    }

    output << stack.top();
    
    return 0;
}

