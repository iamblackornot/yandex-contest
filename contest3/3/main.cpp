#include <fstream>
#include <vector>
#include <cmath>

#define MIN_COORDINATE 0
#define MAX_COORDINATE 2000

struct Point
{
    size_t x;
    size_t y;
};
struct TableParams
{
    size_t tableWidth; 
    size_t tableHeight;
    size_t offsetX;
    size_t offsetY;
};

struct KadaneResult
{
    uint64_t ordersCount;
    size_t start;
    size_t end;
};


KadaneResult KadaneOfK(std::vector<uint64_t> const& arr, size_t k) {

    KadaneResult res;

    uint64_t sum = 0;

    for(size_t i = 0; i < k; ++i) 
    {
        sum += arr[i];
    }

    res.ordersCount = sum;
    res.start = 0;
    res.end = k - 1;

    for(size_t i = k; i < arr.size(); ++i) 
    {
        sum -= arr[i - k];
        sum += arr[i];

        if(sum > res.ordersCount) {
            res.ordersCount = sum;
            res.start = i - k + 1;
            res.end = i;
        }
    }

    return res;
}

class Matrix
{
public:
    Matrix(size_t p_rows, size_t p_cols) : Rows(p_rows), Cols(p_cols)
    {
        buffer = new int64_t[Rows * Cols];
    }
    int64_t& at(size_t row, size_t col)
    {
        if(row < 0 || row >= Rows || col < 0 || col >= Cols)
        {
            throw std::invalid_argument("outside matrix boundaries");
        }
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

class OrderManager
{
public:
    struct HotArea
    {
        Point topLeft;
        Point bottomRight;
        uint64_t orderCount;
    };

    OrderManager(std::vector<Point> const& orders, TableParams tableParams);
    HotArea GetHotArea(uint64_t maxSquare);

private:
    size_t offsetX;
    size_t offsetY;
    Matrix m_orderTable;
};

OrderManager::OrderManager(
    std::vector<Point> const& orders, TableParams tableParams)
    : offsetX(tableParams.offsetX), offsetY(tableParams.offsetY),
    m_orderTable(tableParams.tableHeight, tableParams.tableWidth)
{
    for(size_t row = 0; row < m_orderTable.Rows; ++row)
    {
        for(size_t col = 0; col < m_orderTable.Cols; ++col)
        {
            m_orderTable.at(row, col) = 0;
        }
    }

    for(size_t i = 0; i < orders.size(); ++i)
    {
        ++m_orderTable.at(
            orders[i].y - tableParams.offsetY,
            orders[i].x - tableParams.offsetX);
    }
}

OrderManager::HotArea OrderManager::GetHotArea(uint64_t maxSquare)
{
    HotArea res;
    res.orderCount = 0;
    res.topLeft = { 0, 0 };
    res.bottomRight = { 0, 0 };

    size_t minAreaWidth = 
        m_orderTable.Rows > 1 ? maxSquare / (m_orderTable.Rows - 1) : maxSquare;
    minAreaWidth = std::min(minAreaWidth, m_orderTable.Cols - 1);
    minAreaWidth = std::max(minAreaWidth, (size_t)1);

    // size_t minAreaHeight =
    //     m_orderTable.Cols > 1 ? maxSquare / (m_orderTable.Cols - 1) : maxSquare;
    // minAreaHeight = std::min(minAreaHeight, m_orderTable.Rows - 1);    

    // size_t maxAreaWidth = maxSquare / minAreaWidth;
    // maxAreaWidth = std::min(maxAreaWidth, m_orderTable.Cols - 1);

    // size_t maxAreaHeight = maxSquare / minAreaHeight;
    // maxAreaHeight = std::min(maxAreaHeight, m_orderTable.Rows - 1);

    for(size_t topLeftCol = 0; topLeftCol < m_orderTable.Cols; ++topLeftCol)
    {
        std::vector<uint64_t> colSum(m_orderTable.Rows, 0);

        for(size_t bottomRightCol = topLeftCol; bottomRightCol < m_orderTable.Cols; ++bottomRightCol)
        {
            for(size_t i = 0; i < m_orderTable.Rows; ++i) 
            {
                colSum[i] += m_orderTable.at(i, bottomRightCol);
            }

            size_t areaWidth = bottomRightCol - topLeftCol;
            areaWidth = std::max(areaWidth, size_t(1));
            
            if(areaWidth < minAreaWidth)
            {
                continue;
            }

            size_t areaHeight = maxSquare / areaWidth;

            if(areaHeight < 1)
            {
                break;
            }

            size_t subArrayTargetSize = std::min(areaHeight + 1, m_orderTable.Rows);
            KadaneResult kadaneRes = KadaneOfK(colSum, subArrayTargetSize);

            if(kadaneRes.ordersCount > res.orderCount)
            {
                res.orderCount = kadaneRes.ordersCount;
                res.topLeft = Point{ topLeftCol, kadaneRes.start };
                res.bottomRight = Point{ bottomRightCol, kadaneRes.end };
            }
        }
    }

    res.topLeft = { res.topLeft.x + offsetX, res.topLeft.y + offsetY };
    res.bottomRight = { res.bottomRight.x + offsetX, res.bottomRight.y + offsetY };

    return res;
}

int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    uint64_t orderCount, maxSquare;
    input >> orderCount >> maxSquare;

    std::vector<Point> orders;
    orders.reserve(orderCount);

    size_t minX = MAX_COORDINATE;
    size_t minY = MAX_COORDINATE;
    size_t maxX = MIN_COORDINATE;
    size_t maxY = MIN_COORDINATE;

    for(size_t i = 0; i < orderCount; i++) 
    {
        size_t x, y;
        input >> x >> y;

        orders.push_back(Point{ x, y });

        minX = std::min(minX, x);
        minY = std::min(minY, y);
        maxX = std::max(maxX, x);
        maxY = std::max(maxY, y);
    }

    TableParams params;
    params.offsetX = minX;
    params.offsetY = minY;
    params.tableWidth = maxX - minX + 1;
    params.tableHeight = maxY - minY + 1;

    OrderManager orderManager(orders, params);
    OrderManager::HotArea hotArea = orderManager.GetHotArea(maxSquare);

    output << hotArea.orderCount << std::endl;

    size_t orderNum = 1;

    for(Point const& order : orders)
    {
        if(order.x <= hotArea.bottomRight.x &&
           order.x >= hotArea.topLeft.x &&
           order.y <= hotArea.bottomRight.y &&
           order.y >= hotArea.topLeft.y)
        {
            output << orderNum << " ";
        }

        ++orderNum;
    }

    return 0;
}