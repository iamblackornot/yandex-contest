#include <vector>
#include <memory>
#include <fstream>
#include <cmath>

#define EPSILON 1e-6

bool equalD(double lhs, double rhs)
{
    return fabs(lhs - rhs) < EPSILON;
}
class Matrix
{
public:
    Matrix(size_t p_rows, size_t p_cols) : Rows(p_rows), Cols(p_cols)
    {
        buffer = new double[Rows * Cols];
    }
    double& At(size_t row, size_t col)
    {
        return *(buffer + row * Cols + col);
    }
    double const& At(size_t row, size_t col) const
    {
        return *(buffer + row * Cols + col);
    }
    std::vector<double> Solve();
    double GetMinor(size_t rowToIgnore, size_t colToIgnore) const;
    ~Matrix()
    {
        if(Rows && Cols)
        {
            delete[] buffer;
        }
    }
    void Print()
    {
        std::ofstream outputFile("output.txt", std::fstream::app);
        outputFile.precision(6);

        for(size_t i = 0; i < Rows ; ++i)
        {
            for(size_t j = 0; j < Cols; ++j)
            {
                outputFile << At(i, j) << " ";
            }
            outputFile << std::endl;
        }

        outputFile << std::endl;
        outputFile << std::endl;
    }
private:
    void EliminateRow(size_t row, size_t eqNum, double multiplier);
public:
    size_t Rows;
    size_t Cols;
private:
    double* buffer;
};

class SquareMatrix : public Matrix
{
public:
    SquareMatrix(size_t size) : Matrix(size, size) {}
    double Determinant() const;
};

double Matrix::GetMinor(size_t rowToIgnore, size_t colToIgnore) const
{
    size_t size = std::min(Rows, Cols);

    if(size == 2)
    {
        return At(size - 1 - rowToIgnore, size - 1 - colToIgnore);
    }

    SquareMatrix minorMatrix(size - 1);

    size_t row = 0;

    for(size_t i = 0; i < size; ++i)
    {
        size_t col = 0;

        if(i == rowToIgnore)
        {
            continue;
        }
        for(size_t j = 0; j < size; ++j)
        {
            if(j == colToIgnore)
            {
                continue;
            }

            minorMatrix.At(row, col) = At(i, j);
            ++col;
        }
        ++row;
    }

    return minorMatrix.Determinant();
}

double SquareMatrix::Determinant() const
{
    if(Cols == 1)
    {
        return At(0, 0);
    }

    int koeff = 1;
    double res = 0;

    for(size_t currCol = 0; currCol < Cols; ++currCol)
    {
        res += koeff * At(0, currCol) * GetMinor(0, currCol);
        koeff *= -1;
    }

    return res;
}
void Matrix::EliminateRow(size_t row, size_t eqNum, double multiplier)
{
    for(size_t col = eqNum + 1; col < Cols; ++col)
    {
        At(row, col) = At(row, col) - multiplier * At(eqNum, col); 
    }
}

std::vector<double> Matrix::Solve()
{
    for(size_t eqNum = 0; eqNum < std::min(Cols, Rows); ++eqNum)
    {
        size_t targetRow;
        bool targetRowWasFound = false;

        for(size_t row = eqNum; row < Rows; ++row)
        {
            if(!equalD(At(row, eqNum), 0))
            {
                targetRow = row;
                targetRowWasFound = true;
                break;
            }
        }

        if(!targetRowWasFound)
        {
            continue;
        }

        if(targetRow != eqNum)
        {
            for(size_t col = 0; col < Cols; ++col)
            {
                std::swap(At(eqNum, col), At(targetRow, col));
            }
        }

        for(size_t row = eqNum + 1; row < Rows; ++row)
        {
            double multiplier = At(row, eqNum) / At(eqNum, eqNum);

            if(equalD(multiplier, 0))
            {
                continue;
            }

            At(row, eqNum) = 0;

            
            EliminateRow(row, eqNum, multiplier);
        }
    }
    //Print();
    std::vector<double> result(Rows);

    for(size_t i = Rows; i > 0; --i)
    {
        size_t row = i - 1;

        if(equalD(At(row, row), 0))
        {
            result[row] = 1;
            continue;
        }

        double value = At(row, Cols - 1);

        for(size_t col = row + 1; col < Cols - 1; ++col)
        {
            value -= result[col] * At(row, col);
        }

        result[row] = value / At(row, row);
    }

    return result;
}

int main()
{
    std::ifstream inputFile("input.txt");
    inputFile.precision(12);

    int n;

    inputFile >> n;

    Matrix solutionMatrix(n, n + 1);

    for(size_t i = 0; i < n - 1; ++i)
    {
        for(size_t j = 0; j < n; ++j)
        {
            inputFile >> solutionMatrix.At(i, j);
        }
    }

    int koeff = (n - 1) & 1 ? - 1 : 1;

    for(size_t col = 0; col < n; ++col)
    {
        solutionMatrix.At(n - 1, col) = koeff * solutionMatrix.GetMinor(n - 1, col);
        koeff *= -1;
    }

    for(size_t row = 0; row < n - 1; ++row)
    {
        solutionMatrix.At(row, n) = 0;
    }

    solutionMatrix.At(n - 1, n) = 1;

    //solutionMatrix.Print();

    auto&& res = solutionMatrix.Solve();

    //solutionMatrix.Print();

    std::ofstream outputFile("output.txt");
    outputFile.precision(12);

    for(int i = 0; i < n; ++i)
    {
        outputFile << res[i] << " ";
    }

    return 0;
}