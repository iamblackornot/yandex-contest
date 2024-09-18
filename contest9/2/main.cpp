#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <string>

std::string Shorten(std::string source, size_t prefixSize)
{
    if( (prefixSize * 2 > source.size()) || (source.size() - prefixSize * 2 <= 1) )
    {
         return source; 
    }

    return 
        source.substr(0, prefixSize) + 
        std::to_string(source.size() - prefixSize * 2) + 
        source.substr(source.size() - prefixSize, prefixSize);
}

class QueueItem
{
public:
    size_t stringIndex;
    size_t prefixSize;

    QueueItem(size_t stringIndex, size_t prefixSize = 1) 
        : stringIndex(stringIndex), prefixSize(prefixSize) {}
    
    bool operator< (QueueItem const& rhs)
    {
        return this->prefixSize > rhs.prefixSize;
    }
};

class Solution
{
public:
    void Add(std::string word)
    {
        words.push_back(word);
        processQueue.emplace(words.size() - 1);
    };
    void Process();
    void Print(std::ostream& output) const;

private:
    std::vector<std::string> words;
    std::unordered_map<std::string, size_t> contractionToId;
    std::unordered_map<size_t, std::string> idToContraction;
    std::unordered_set<std::string> forbiddenContractions;
    std::queue<QueueItem> processQueue;
};

void Solution::Process()
{
    while(!processQueue.empty())
    {
        QueueItem const& nextItem = processQueue.front();
        std::string contraction = Shorten(words[nextItem.stringIndex], nextItem.prefixSize);

        if(contractionToId.contains(contraction))
        {
            processQueue.emplace(nextItem.stringIndex, nextItem.prefixSize + 1);
            processQueue.emplace(contractionToId.at(contraction), nextItem.prefixSize + 1);
            forbiddenContractions.insert(contraction);
            contractionToId.erase(contraction);
        }
        else if(forbiddenContractions.contains(contraction))
        {
            processQueue.emplace(nextItem.stringIndex, nextItem.prefixSize + 1);
        } 
        else
        {
            contractionToId.insert(std::make_pair(contraction, nextItem.stringIndex));
            idToContraction[nextItem.stringIndex] = contraction;
        }

        processQueue.pop();
    }
}

void Solution::Print(std::ostream& output) const
{
    for(size_t i = 0; i < words.size(); ++i) 
    {
        output << idToContraction.at(i) << std::endl;
    }
}

int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    size_t count;
    input >> count;

    Solution sol;

    for(size_t i = 0; i < count; ++i) 
    {
        std::string word;
        input >> word;

        sol.Add(std::move(word));
    }

    sol.Process();
    sol.Print(output);

    return 0;
}