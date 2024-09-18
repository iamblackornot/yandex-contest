#include <fstream>
#include <vector>

class Matrix
{
public:
    Matrix(size_t p_rows, size_t p_cols) : Rows(p_rows), Cols(p_cols)
    {
        buffer = new int64_t[Rows * Cols];
    }
    int64_t& At(size_t row, size_t col)
    {
        return *(buffer + row * Cols + col);
    }
    int64_t const& At(size_t row, size_t col) const
    {
        return *(buffer + row * Cols + col);
    } 
    ~Matrix()
    {
        delete[] buffer;
    }
public:
    size_t Rows;
    size_t Cols;
private:
    int64_t* buffer;
};

int64_t maxSubArraySumLengthK(std::vector<int64_t> const& nums, size_t k)
{
    int64_t sum = 0;

    for(size_t i = 0; i < k; ++i)
    {
        sum += nums[i];
    }

    int64_t maxSum = sum;

    for(size_t i = k; i < nums.size(); ++i)
    {
        sum -= nums[i - k];
        sum += nums[i];

        maxSum = std::max(maxSum, sum);
    }

    return maxSum;
}

int64_t maxSubArraySumLengthLeqK(std::vector<int64_t> const& nums, size_t k)
{
    if(nums.size() < k || nums.size() == 0)
    {
        return 0;
    }

    int64_t max = nums[0];

    for(size_t i = 1; i <= k; ++i)
    {
        max = std::max(max, maxSubArraySumLengthK(nums, i));
    }

    return max;
}

int main()
{
    std::ifstream inputFile("input.txt");

    size_t k;
    inputFile >> k;

    size_t cols, rows;
    inputFile >> rows >> cols;

    Matrix matrix(rows, cols);

    for(size_t row = 0; row < rows; ++row)
    {
        for(size_t col = 0; col < cols; ++ col)
        {
            inputFile >> matrix.At(row, col);
        }
    }

    std::vector<int64_t> rowSum(rows, 0);
    int64_t maxRectSum = matrix.At(0, 0);

    for(size_t upperLeftCol = 0; upperLeftCol < cols; ++upperLeftCol)
    {
        std::vector<int64_t> rowSum(rows, 0);

        for(size_t downRightCol = upperLeftCol; downRightCol < cols; ++downRightCol)
        {
            size_t rectWidth = downRightCol - upperLeftCol + 1;
            size_t rectMaxHeight = k / rectWidth;

            if(rectMaxHeight == 0)
            {
                continue;
            }

            rectMaxHeight = std::min(rectMaxHeight, rows);

            for(size_t row = 0; row < rows; ++row)
            {
                rowSum[row] += matrix.At(row, downRightCol);
            }


            maxRectSum = std::max(maxRectSum, maxSubArraySumLengthLeqK(rowSum, rectMaxHeight));
        }
    }

    std::ofstream outputFile("output.txt");
    outputFile << maxRectSum;

    return 0;
}