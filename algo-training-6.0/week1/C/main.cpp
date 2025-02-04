#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <fstream>

constexpr int MIN_SIZE = 0;
constexpr int MAX_SIZE = 10;

struct Rect
{
    int botX = MAX_SIZE;
    int botY = MIN_SIZE;
    int topX = MIN_SIZE;
    int topY = MAX_SIZE;

    bool isValid() { return botX < topX && topY < botY; }
};

class Board
{
public:
    Board(int size) : size(size), diodes(size, std::string(size, '.')) {}
    Rect GetLightRect();
    char ParseSymbol();
    std::vector<Rect> ParseDarkSubrects(Rect const& lightRect);
private:
    bool ColumnsEqual(std::vector<std::string> const& board, int col1, int col2);
    std::vector<std::string> Truncate(Rect const& lightRect);
    std::vector<std::string> CompressV(std::vector<std::string> const& board);
    std::vector<std::string> CompressH(std::vector<std::string> const& board);

    bool SearchI(std::vector<std::string> const& board);
    bool SearchL(std::vector<std::string> const& board);
    bool SearchC(std::vector<std::string> const& board);
    bool SearchO(std::vector<std::string> const& board);
    bool SearchH(std::vector<std::string> const& board);
    bool SearchP(std::vector<std::string> const& board);
private:
    int size;
    std::vector<std::string> diodes;

    friend std::istream& operator>> (std::istream& input, Board& board);
    friend std::ostream& operator<< (std::ostream& input, Board& board);
};

Rect Board::GetLightRect()
{
    Rect res;

    for(int row = 0; row < size; ++row)
    {
        for(int col = 0; col < size; ++col)
        {
            if(diodes[row][col] == '#')
            {
                res.botX = std::min(res.botX, col);
                res.botY = std::max(res.botY, row + 1);
                res.topX = std::max(res.topX, col + 1);
                res.topY = std::min(res.topY, row);
            }
        }
    }

    return res;
}

std::vector<std::string> Board::Truncate(Rect const& lightRect)
{
    int truncatedRows = lightRect.botY - lightRect.topY;
    int truncatedCols = lightRect.topX - lightRect.botX;

    std::vector<std::string> truncated(truncatedRows, std::string(truncatedCols, '.'));

    for(int row = 0; row < truncatedRows; ++row)
    {
        for(int col = 0; col < truncatedCols; ++col)
        {
            truncated[row][col] = diodes[row + lightRect.topY][col + lightRect.botX];
        }
    }

    return truncated;
}

std::vector<std::string> Board::CompressV(std::vector<std::string> const& board)
{
    std::vector<std::string> compressed;
    int i = 0;

    for(int row = 1; row < board.size(); ++row)
    {
        if(board[row] != board[i])
        {
            compressed.push_back(board[i]);
            i = row;
        }
    }

    compressed.push_back(board[i]);

    return compressed;
}

bool Board::ColumnsEqual(std::vector<std::string> const& board, int col1, int col2)
{
    for(int row = 0; row < board.size(); ++row)
    {
        if(board[row][col1] != board[row][col2]) { return false; }
    }

    return true;
}

std::vector<std::string> Board::CompressH(std::vector<std::string> const& board)
{
    std::vector<std::string> compressed(board.size());
    int i = 0;

    for(int col = 1; col < board[0].size(); ++col)
    {
        if(!ColumnsEqual(board, col, i))
        {
            for(int row = 0; row < compressed.size(); ++row)
            {
                compressed[row].push_back(board[row][i]);
            }

            i = col;
        }
    }

    for(int row = 0; row < compressed.size(); ++row)
    {
        compressed[row].push_back(board[row][i]);
    }

    return compressed;
}

bool Board::SearchI(std::vector<std::string> const& board)
{
    return board.size() == 1 && board[0].size() == 1 && board[0][0] == '#';
}
bool Board::SearchL(std::vector<std::string> const& board)
{
    return board.size() == 2 
        && board[0].size() == 2
        && board[0][0] == '#'
        && board[0][1] == '.'
        && board[1][0] == '#'
        && board[1][1] == '#';
}

bool Board::SearchC(std::vector<std::string> const& board)
{
    return board.size() == 3 
        && board[0].size() == 2
        && board[0][0] == '#'
        && board[0][1] == '#'
        && board[1][0] == '#'
        && board[1][1] == '.'
        && board[2][0] == '#'
        && board[2][1] == '#';
}
bool Board::SearchO(std::vector<std::string> const& board)
{
    return board.size() == 3 
        && board[0].size() == 3
        && board[0][0] == '#'
        && board[0][1] == '#'
        && board[0][2] == '#'
        && board[1][0] == '#'
        && board[1][1] == '.'
        && board[1][2] == '#'
        && board[2][0] == '#'
        && board[2][1] == '#'
        && board[2][2] == '#';
}
bool Board::SearchH(std::vector<std::string> const& board)
{
    return board.size() == 3 
        && board[0].size() == 3
        && board[0][0] == '#'
        && board[0][1] == '.'
        && board[0][2] == '#'
        && board[1][0] == '#'
        && board[1][1] == '#'
        && board[1][2] == '#'
        && board[2][0] == '#'
        && board[2][1] == '.'
        && board[2][2] == '#';
}
bool Board::SearchP(std::vector<std::string> const& board)
{
    return board.size() == 4 
        && board[0].size() == 3
        && board[0][0] == '#'
        && board[0][1] == '#'
        && board[0][2] == '#'
        && board[1][0] == '#'
        && board[1][1] == '.'
        && board[1][2] == '#'
        && board[2][0] == '#'
        && board[2][1] == '#'
        && board[2][2] == '#'
        && board[3][0] == '#'
        && board[3][1] == '.'
        && board[3][2] == '.';
}


char Board::ParseSymbol()
{
    Rect lightRect  = GetLightRect();

    if(!lightRect.isValid()) { return 'X'; }

    std::vector<std::string> truncated = Truncate(lightRect);
    std::vector<std::string> compressedV = CompressV(truncated);
    std::vector<std::string> compressed = CompressH(compressedV);

    if(SearchI(compressed)) { return 'I'; }
    if(SearchL(compressed)) { return 'L'; }
    if(SearchC(compressed)) { return 'C'; }
    if(SearchO(compressed)) { return 'O'; }
    if(SearchH(compressed)) { return 'H'; }
    if(SearchP(compressed)) { return 'P'; }

    // for(int i = 0; i < compressed.size(); ++i)
    // {
    //     std::cout << compressed[i] << std::endl;
    // }

    // std::cout << std::endl << std::endl;

    return 'X';
}

inline std::istream& operator>> (std::istream& input, Board& board)
{
    for(int i = 0; i < board.size; ++i)
    {
        input >> board.diodes[i];
    }
    
    return input;
}

inline std::ostream& operator<< (std::ostream& output, Board& board)
{
    for(int i = 0; i < board.size; ++i)
    {
        output << board.diodes[i] << std::endl;
    }

    return output;
}

bool RunTests()
{
    std::ifstream input("tests.txt");

    while(!input.eof())
    {
        int size;
        input >> size;

        char expected;
        input >> expected;

        Board board(size);
        input >> board;

        char actual = board.ParseSymbol();
        
        if(expected != actual)
        {
            std::cout << "test failed" << std::endl;
            std::cout << board; 
            std::cout << "expected = " << expected << std::endl;
            std::cout << "actual = " << actual << std::endl;

            return false;
        }
    }

    std::cout << "all tests passed" << std::endl;

    return true;
}

int main()
{
    //if(!RunTests()) { return 0; }

    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int size;
    input >> size;

    Board board(size);
    input >> board;
    output << board.ParseSymbol();

    return 0;
}