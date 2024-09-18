#include <fstream>
#include <iostream>
#include <algorithm>
#include <stack>
#include <variant>
#include <optional>
#include <list>

using CharBufIter = std::istreambuf_iterator<char>;
using StopCharOpt = std::optional<char>;

class TokenGroup;

struct SpaceToken
{
    std::ostream& Print(std::ostream& output) const
    {
        output << " ";
        return output;
    }
};

class LiteralToken
{
public:
    LiteralToken(std::string&& literal) : m_literal(literal) {}
    std::string GetLiteral() const { return m_literal; };
    std::ostream& Print(std::ostream& output) const
    {
        output << m_literal;
        return output;
    }
private:
        std::string m_literal;
};


class SpecialCharacterToken
{
public:
    SpecialCharacterToken(char ch) : m_ch(ch) {}
    char GetCharacter() const { return m_ch; }
    std::ostream& Print(std::ostream& output) const
    {
        output << m_ch;
        return output;
    }
private:
    char m_ch;
};

struct ParameterToken
{
    std::ostream& Print(std::ostream& output) const
    {
        output << "?";
        return output;
    }
};

struct RangeParameterToken
{
    std::ostream& Print(std::ostream& output) const
    {
        output << "...";
        return output;
    }
};

using Token = std::variant<
    SpaceToken, 
    LiteralToken, 
    SpecialCharacterToken, 
    ParameterToken,
    RangeParameterToken, 
    TokenGroup
>;

class TokenGroup
{
public:
    TokenGroup() = default;
    TokenGroup(TokenGroup&& rhs);
    std::list<Token>::iterator begin() { return m_tokens.begin(); }
    std::list<Token>::iterator end() { return m_tokens.end(); }
    void push_back(Token el) { m_tokens.push_back(std::move(el)); }
    void Normalize();
    std::ostream& Print(std::ostream& output) const;
private:
    std::list<Token> m_tokens;
};

auto GetSearchLiteralTokenPredicate(std::string searchFor)
{
    return [searchFor](Token const& el) 
        { 
            LiteralToken const* token = std::get_if<LiteralToken>(&el);
            return token != nullptr && token->GetLiteral() == searchFor;
        };
}

bool IsCharacterToken(Token const& token, char ch)
{
    SpecialCharacterToken const* sct = std::get_if<SpecialCharacterToken>(&token);

    if(sct && sct->GetCharacter() == ch)
    {
        return true;
    }

    return false;
}

TokenGroup::TokenGroup(TokenGroup&& rhs)
{
    m_tokens = std::move(rhs.m_tokens);
}

void TokenGroup::Normalize()
{
    auto it = m_tokens.begin();

    while(it != m_tokens.end() && std::holds_alternative<SpaceToken>(*it))
    {
        it = m_tokens.erase(it);
    }

    auto rit = m_tokens.rbegin();

    while(rit != m_tokens.rend() && std::holds_alternative<SpaceToken>(*rit))
    {
        std::advance(rit, 1);
        m_tokens.erase(rit.base());
    }

    // auto whereIt = std::find_if(m_tokens.begin(), m_tokens.end(), GetSearchLiteralTokenPredicate("where"));

    // if(whereIt == m_tokens.end())
    // {
    //     return;
    // }

    auto inIt = std::find_if(m_tokens.begin(), m_tokens.end(), GetSearchLiteralTokenPredicate("in"));

    while(inIt != m_tokens.end())
    {
        ++inIt;

        if(!IsCharacterToken(*inIt, '('))
        {
            ++inIt;

            if(!IsCharacterToken(*inIt, '('))
            {
                continue;;
            }
        }

        ++inIt;

        while(!IsCharacterToken(*inIt, ')'))
        {
            inIt = m_tokens.erase(inIt);
        }

        inIt = m_tokens.insert(inIt, RangeParameterToken{});
        std::advance(inIt, 2);

        inIt = std::find_if(inIt, m_tokens.end(), GetSearchLiteralTokenPredicate("in"));
    }

    return;
}

std::ostream& TokenGroup::Print(std::ostream& output) const
{
    auto execute = [this, &output](auto&& token) { token.Print(output); };

    for(auto&& token : m_tokens)
    {
        std::visit(execute, token);
    }

    return output;
}

struct DefaultTokenizerState;
struct SpaceTokenizerState;
struct LiteralTokenizerState;
struct GroupTokenizerState;
struct ParameterTokenizerState;
struct FinishedTokenizerState;

using TokenizerState = std::variant<
    DefaultTokenizerState,
    SpaceTokenizerState,
    LiteralTokenizerState,
    GroupTokenizerState,
    ParameterTokenizerState,
    FinishedTokenizerState
>;

struct DefaultTokenizerState
{
    TokenizerState Proceed(CharBufIter& it, TokenGroup& out, StopCharOpt stopChar);   
};

struct SpaceTokenizerState
{
    TokenizerState Proceed(CharBufIter& it, TokenGroup& out, StopCharOpt stopChar);  
};

struct LiteralTokenizerState
{
    TokenizerState Proceed(CharBufIter& it, TokenGroup& out, StopCharOpt stopChar);  
};

struct GroupTokenizerState
{
    TokenizerState Proceed(CharBufIter& it, TokenGroup& out, StopCharOpt stopChar);  
};
struct ParameterTokenizerState
{
    TokenizerState Proceed(CharBufIter& it, TokenGroup& out, StopCharOpt stopChar);  
};

struct FinishedTokenizerState
{
    TokenizerState Proceed(CharBufIter& it, TokenGroup& out, StopCharOpt stopChar) { return *this; } 
};

class Tokenizer
{
public:
    Tokenizer(CharBufIter& input, StopCharOpt stopChar = std::nullopt) 
        : m_input(input), m_stopChar(std::move(stopChar)) {}
    TokenGroup ReadTokens();
private:
    CharBufIter& m_input;
    StopCharOpt m_stopChar;
    TokenizerState m_state = DefaultTokenizerState{};
    TokenGroup m_tokens;
};

TokenGroup Tokenizer::ReadTokens()
{
    while(!std::holds_alternative<FinishedTokenizerState>(m_state))
    {
        auto execute = [this](auto state) -> TokenizerState{ return state.Proceed(m_input, m_tokens, m_stopChar); };
        m_state = std::visit(execute, m_state);
    }

    m_tokens.Normalize();

    return std::move(m_tokens);
}

TokenizerState DefaultTokenizerState::Proceed(CharBufIter& it, TokenGroup& out, StopCharOpt stopChar)
{
    if(it == CharBufIter())
    {
        return FinishedTokenizerState{};
    }

    if(stopChar && *it == stopChar)
    {
        ++it;
        return FinishedTokenizerState{};
    }
    
    switch (*it)
    {
    case '(':
        return GroupTokenizerState{};
    case '"':
        return ParameterTokenizerState{};
        break;
    case '_':
        return LiteralTokenizerState{};
        break;
    default:
        {
            if(std::isspace(*it))
            {
                return SpaceTokenizerState{};
            }
            if(std::isalnum(*it))
            {
                return LiteralTokenizerState{};
            }

            out.push_back(SpecialCharacterToken(*it));
            ++it;
            return DefaultTokenizerState{};

            break;
        }
    }
}

TokenizerState SpaceTokenizerState::Proceed(CharBufIter& it, TokenGroup& out, StopCharOpt stopChar)
{
    while(it != CharBufIter() && std::isspace(*it))
    {
        ++it;
    }

    out.push_back(SpaceToken{});
    return DefaultTokenizerState{};
}

TokenizerState LiteralTokenizerState::Proceed(CharBufIter& it, TokenGroup& out, StopCharOpt stopChar)
{
    std::string literal;

    while(it != CharBufIter() && (
        std::isalnum(*it) || *it == '_' || *it == '.'))
    {
        literal.push_back(std::tolower(*it));
        ++it;
    }

    out.push_back(LiteralToken(std::move(literal)));
    return DefaultTokenizerState{};
}

TokenizerState GroupTokenizerState::Proceed(CharBufIter& it, TokenGroup& out, StopCharOpt stopChar)
{
    ++it;
    Tokenizer tokenizer(it, ')');

    out.push_back(SpecialCharacterToken('('));

    auto tokenGroup = tokenizer.ReadTokens();
    tokenGroup.Normalize();

    out.push_back(std::move(tokenGroup));
    out.push_back(SpecialCharacterToken(')'));

    return DefaultTokenizerState{};
}
TokenizerState ParameterTokenizerState::Proceed(CharBufIter& it, TokenGroup& out, StopCharOpt stopChar)
{
    ++it;

    while(it != CharBufIter() && *it != '"')
    {
        ++it;
    }

    ++it;
    out.push_back(ParameterToken{});

    return DefaultTokenizerState{};
}


int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    CharBufIter iter{ input };
    Tokenizer tokenizer(iter, std::nullopt);

    tokenizer.ReadTokens().Print(output);

    return 0;
}