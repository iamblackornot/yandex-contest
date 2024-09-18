#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <limits>
#include <string>


struct Staff
{
    //int id;
    std::string Name;
    int JoinedSince = 0;
    unsigned int Experience = 0;

    Staff(std::string name, int joinedSince)
        : Name(name), JoinedSince(joinedSince) {}
};

typedef std::vector<Staff>::iterator StaffIt;

struct StaffItComp
{
    bool operator()(StaffIt lhs, StaffIt rhs) const
    {
        int64_t expDiff = (int64_t) (lhs->Experience - rhs->Experience);
        expDiff -= lhs->JoinedSince - rhs->JoinedSince;

        if(expDiff == 0)
            return lhs->Name < rhs->Name;

        return expDiff > 0;

        // if(lhs->Experience == rhs->Experience) 
        //     return lhs->Name < rhs->Name;

        // return lhs->Experience > rhs->Experience;
    }
};


typedef std::set<StaffIt, StaffItComp>::iterator StaffExpIt;




class StaffManager
{
    static const int maxWorkers = 300'000;
public:

    StaffManager() { _staff.reserve(maxWorkers); }
    void ProcessPerson(std::string name, uint64_t time)
    {
        uint64_t expGained = time - _lastTimeUpdated;
        totalExperience += _activeWorkers.size() * expGained;

        // for(auto const&[id, it] : _activeWorkers)
        // {
        //     _staff[id].Experience += expGained;
        // }

        _lastTimeUpdated = time;

        int id = GetWorkerId(name, time);

        if(_activeWorkers.count(id) == 0)
        {
            _staff.at(id).JoinedSince = time;
            auto res = _sortedExperience.insert( std::next(_staff.begin(), id) );
            _activeWorkers.insert( { id, res.first } );
            totalExperience += _staff.at(id).Experience;
        }
        else
        {
            auto expIt = _activeWorkers.at(id);
            _sortedExperience.erase(expIt);
            _activeWorkers.erase(id);

            Staff& staff = _staff.at(id);
            staff.Experience += time - staff.JoinedSince;

            totalExperience -= staff.Experience;
        }
    }
    void PrintMostExperienced(std::ostream& output, int time)
    {
        auto staffIt = *_sortedExperience.begin();
        uint64_t topStaffExp = staffIt->Experience + (time - staffIt->JoinedSince);
        output << staffIt->Name << " " << (int64_t) (totalExperience - 2 * topStaffExp) << "\n";
        //output << staffIt->Name << " " << totalExperience - 2 * staffIt->Experience << "\n";
    }
private:
    int GetWorkerId(std::string name, int time)
    {
        if(_nameToId.count(name) > 0) return _nameToId.at(name);

        _staff.emplace_back(name, time);
        int newId = _staff.size() - 1;
        _nameToId.insert( { name, newId } );

        return newId;
    }
private:
    uint64_t _lastTimeUpdated;
    uint64_t totalExperience;
    std::vector<Staff> _staff;
    std::unordered_map<std::string, int> _nameToId;
    std::unordered_map<int, StaffExpIt> _activeWorkers;
    std::set<StaffIt, StaffItComp> _sortedExperience;
};

int main() 
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int count;
    input >> count;

    StaffManager manager;

    for(size_t i = 0; i < count; ++i)
    {
        std::string name;
        int time;

        input >> name;
        input >> time;

        manager.ProcessPerson(name, time);
        manager.PrintMostExperienced(output, time);
    }

    return 0;
}