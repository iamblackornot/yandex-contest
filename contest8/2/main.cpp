#include <fstream>
#include <unordered_map>
#include <set>

struct ArithmProgression 
{
    int64_t start;
    int increment;
    int id;

public:
    ArithmProgression(int64_t start, int increment, int id);
    ArithmProgression Increment() const;
    bool operator<(ArithmProgression const& rhs) const;
};

ArithmProgression::ArithmProgression(int64_t start, int increment, int id) : start(start), increment(increment), id(id) {}

ArithmProgression ArithmProgression::Increment() const
{
    return ArithmProgression(this->start + this->increment, this->increment, this->id);
}

bool ArithmProgression::operator<(ArithmProgression const& rhs) const
{
    if(this->start == rhs.start) 
    { 
        return this->id < rhs.id; 
    }

    return this->start < rhs.start;
}

class ArithmProgressionContainer
{
    typedef std::set<ArithmProgression>::iterator QueueIt;
public:
    void Add(int64_t start, int increment, int id);
    void Remove(int id);
    void Min(std::ostream& output);
private:
    std::set<ArithmProgression> queue;
    std::unordered_map<int, QueueIt> idToIter;
};

void ArithmProgressionContainer::Add(int64_t start, int increment, int id)
{
    auto const& [it, success] = queue.emplace(start, increment, id);

    if(success)
    {
        idToIter.insert(std::make_pair(id, it));
    }
}

void ArithmProgressionContainer::Remove(int id)
{
    queue.erase(idToIter.at(id));
    idToIter.erase(id);
}

void ArithmProgressionContainer::Min(std::ostream& output)
{
    ArithmProgression const min = *queue.begin();
    output << min.start << std::endl;
    
    Remove(min.id);
    Add(min.start + min.increment, min.increment, min.id);
}

int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int opCount;
    input >> opCount;

    ArithmProgressionContainer container;

    for(int i = 0; i < opCount; ++i) 
    {
        int opCode;
        input >> opCode;

        switch (opCode)
        {
        case 1:
            int start;
            int increment;
            int id;

            input >> start;
            input >> increment; 
            input >> id;

            container.Add(start, increment, id);
            break;
        case 2:
            int idToRemove;
            input >> idToRemove;
            container.Remove(idToRemove);
            break;
        case 3:
            container.Min(output);
        default:
            break;
        }
    }

    return 0;
}