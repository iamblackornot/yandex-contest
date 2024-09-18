#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>

class Photo
{
public:
    Photo(int width, int height);
    void Scale(int targetWidth);
    int GetHeight() const;
private:
    int m_width;
    int m_height;
};

Photo::Photo(int width, int height)
    : m_width(width), m_height(height) {}

void Photo::Scale(int targetWidth)
{
    m_height = std::ceil(m_height * targetWidth / (double)(m_width));
    m_width = targetWidth;
}

int Photo::GetHeight() const
{
    return m_height;
}

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int targetWidth, photoCount, ribbonSize;
    input >> targetWidth >> photoCount >> ribbonSize;

    std::vector<Photo> photos;

    for (size_t i = 0; i < photoCount; ++i)
    {
        std::string line;
        input >> line;
        size_t delimiterPos = line.find_first_of('x');

        int width = std::stoi(line.substr(0, delimiterPos));
        int height = std::stoi(line.substr(delimiterPos + 1, line.size() - delimiterPos - 1));
        photos.emplace_back(width, height);
    }

    for(auto& photo : photos)
    {
        photo.Scale(targetWidth);
    }

    std::sort(photos.begin(), photos.end(), 
        [](Photo const& lhs, Photo const& rhs) 
        {
            return lhs.GetHeight() < rhs.GetHeight();
        });

    uint64_t minSum = 0; 

    for(size_t i = 0; i < ribbonSize; ++i)
    {
        minSum += photos[i].GetHeight();
    }

    uint64_t maxSum = 0; 

    for(size_t i = photoCount - ribbonSize; i < photoCount; ++i)
    {
        maxSum += photos[i].GetHeight();
    }

    output << minSum << std::endl << maxSum;

    return 0;
}