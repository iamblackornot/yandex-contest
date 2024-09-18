#include <fstream>
#include <vector>
#include <iostream>

class MineField
{
    enum class Cell
    {
        Unopened,
        Opened,
        Mined
    };

    public:
        MineField(size_t p_rows, size_t p_columns);
        Cell& At(size_t row, size_t col);
        void SetMine(size_t row, size_t col);
        uint64_t GetNumberOfTurnsToOpenAll();
        void Print();
    private:
        void OpenCell(size_t row, size_t col);
    public:
        size_t Rows;
        size_t Columns;
    private:
        Cell* m_buffer;

};

void MineField::Print()
{
    for(size_t row = 0; row < Rows; ++row)
    {
        for(size_t col = 0; col < Columns; ++col)
        {
            switch(At(row, col))
            {
                case Cell::Unopened:
                case Cell::Opened:
                {
                    std::cout << "o";
                    break;
                }
                case Cell::Mined:
                {
                    std::cout << "M";
                    break;
                }
            }
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << std::endl;
}

uint64_t MineField::GetNumberOfTurnsToOpenAll()
{
    uint64_t turnsCount = 0;

    for(size_t row = 0; row < Rows; ++row)
    {
        for(size_t col = 0; col < Columns; ++col)
        {
            if(At(row, col) == Cell::Unopened)
            {
                OpenCell(row, col);
                ++turnsCount;
                Print();
            }
        }
    }

    return turnsCount;
}

void MineField::OpenCell(size_t row, size_t col)
{
    if(row < 0 || col < 0 || row >= Rows || col >= Columns)
    {
        return;
    } 

    auto& cell = At(row, col);

    if(cell != Cell::Unopened)
    {
        return;
    }

    cell = Cell::Opened;

    OpenCell(row, col + 1);
    OpenCell(row, col - 1);
    OpenCell(row + 1, col);
    OpenCell(row - 1, col);
}

void MineField::SetMine(size_t row, size_t col)
{
    At(row, col) = Cell::Mined;
}

MineField::Cell& MineField::At(size_t row, size_t col)
{
    return *(m_buffer + row * Columns + col);
}

MineField::MineField(size_t p_rows, size_t p_columns)
    : Rows(p_rows), Columns(p_columns)
{
    m_buffer = new Cell[Columns * Rows];

    for(size_t row = 0; row < Rows; ++row)
    {
        for(size_t col = 0; col < Columns; ++col)
        {
            At(row, col) = Cell::Unopened;
        }
    }
}

int main()
{
    std::ifstream input("input.txt");

    size_t rows, cols, minesCount;
    input >> rows >> cols >> minesCount;

    MineField field(rows, cols);

    for(size_t i = 0; i < minesCount; ++i)
    {
        size_t row, col;
        input >> row >> col;

        field.SetMine(row - 1, col - 1);
    }

    std::ofstream output("output.txt");
    output << field.GetNumberOfTurnsToOpenAll();

    return 0;
}