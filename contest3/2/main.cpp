#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>

constexpr int DAY_LENGTH_IN_SECONDS = 60 * 60 * 24;

time_t GetTimeT(int day, int month, int year = 2022)
{
    std::tm tm = {0};
    tm.tm_mday = day;
    tm.tm_mon = month - 1;
    tm.tm_year = year - 1900;
    return std::mktime(&tm);
}
struct Quarter
{
    time_t dateStart;
    time_t dateEnd;
};

struct Calendar
{
    constexpr static int QUARTER_COUNT = 4;
    const static Quarter quarters[QUARTER_COUNT];
};

const Quarter Calendar::quarters[Calendar::QUARTER_COUNT] = 
{
    { GetTimeT(1, 1), GetTimeT(31, 3) },
    { GetTimeT(1, 4), GetTimeT(30, 6) },
    { GetTimeT(1, 7), GetTimeT(30, 9) },
    { GetTimeT(1, 10), GetTimeT(31, 12) },
};

struct Invoice
{
    time_t dateFrom;
    time_t dateFinish;
    int64_t dailyCost;
};


std::string CentsToDollarString(int64_t value)
{
    std::stringstream ss;
    std::string str = std::to_string(value);

    if(str.size() > 2)
    {
        ss 
            << str.substr(0, str.size() - 2)
            << "." 
            << str.substr(str.size() - 2, 2);
    } 
    else if(str.size() == 2)
    {
        ss << "0." << str;
    } 
    else if(str.size() == 1)
    {
        ss << "0.0" << str;
    }

    return ss.str();
}

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    size_t invoiceCount;
    input >> invoiceCount;

    std::vector<Invoice> invoices;

    for(size_t i = 0; i < invoiceCount; ++i)
    {
        int amount;
        input >> amount;

        int day, month;
        char delimiter;

        input >> day >> delimiter >> month;
        time_t dateFrom = GetTimeT(day, month);

        input >> day >> delimiter >> month;
        time_t dateFinish = GetTimeT(day, month);

        int days = std::difftime(dateFinish, dateFrom) / DAY_LENGTH_IN_SECONDS;

        int64_t dailyCost = std::floor(amount * 100 / (double)(days + 1));

        invoices.push_back(Invoice
            { 
                .dateFrom = dateFrom,
                .dateFinish = dateFinish,
                .dailyCost = dailyCost,
            });
    }

    std::vector<int64_t> quarterCosts(Calendar::QUARTER_COUNT, 0);

    for(auto const& invoice : invoices)
    {
        for(size_t i = 0; i < Calendar::QUARTER_COUNT; ++i)
        {
            time_t start = std::max(Calendar::quarters[i].dateStart, invoice.dateFrom);
            time_t end = std::min(Calendar::quarters[i].dateEnd, invoice.dateFinish);

            time_t diff = end - start;

            if(diff >= 0)
            {
                int days = diff / DAY_LENGTH_IN_SECONDS;
                quarterCosts[i] += (days + 1) * invoice.dailyCost;
            }
        }
    }

    for(auto const& cost : quarterCosts)
    {
        output << CentsToDollarString(cost) << std::endl;
    }

    return 0;
}