#include <cstdint>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>

typedef std::vector<int> NodeList;
typedef std::unordered_map<int, NodeList> AdjacencyMap;

struct QueueEntry
{
    int to;
    int from;
};

int fact(int val, int modulo)
{
    static std::vector<int> cache { 1, 1, 2 };

    if(cache.size() <= val)
    {
        for(uint64_t i = cache.size(); i <= val; ++i)
        {
            cache.push_back(i * cache.back() % modulo);
        }
    }

    return cache[val];
}

int main()
{
    std::ifstream input("input.txt");
    // std::ifstream input("test17-79531.txt");
    std::ofstream output("output.txt");

    int birdCount;
    int connectCount;
    int modulo;

    input >> birdCount;
    input >> connectCount;
    input >> modulo;

    AdjacencyMap adj;
    std::vector<bool> visitted(birdCount, false);

    for(int i = 0; i < connectCount; ++i)
    {
        int first;
        int second;

        input >> first;
        input >> second;

        adj[first].push_back(second);
        adj[second].push_back(first);
    }

    std::vector<int> hubCount(birdCount, 0);

    for(int i = 1; i <= birdCount; ++i)
    {
        int count = 0;
        
        if(adj.contains(i)) 
        {
            NodeList const& nodes = adj.at(i);

            for(int node : nodes)
            {
                if(adj.contains(node) && adj.at(node).size() > 1) { ++count; }
            }
        }

        hubCount[i - 1] = count;

        if(count > 2) 
        { 
            output << 0;
            return 0;
        }
    }

    uint64_t combs = 1;
    uint64_t exp = 1;
    int segCount = 0;
    int freeBirdCount = 0;

    for(int i = 1; i <= birdCount; ++i)
    {
        if(visitted[i - 1]) { continue; }

        visitted[i - 1] = true;

        if(!adj.contains(i))
        {
            ++freeBirdCount;
            continue;
        }

        ++segCount;
        exp = exp * 2 % modulo;

        std::queue<QueueEntry> queue;
        queue.push( QueueEntry{ .to = i, .from = i } );

        int nodeCount = 0;
        int segHubCount = 0;

        while (!queue.empty())
        {
            int node = queue.front().to;
            int from = queue.front().from;

            queue.pop();

            ++nodeCount;
            visitted[node - 1] = true;

            if(adj.contains(node)) 
            {
                NodeList const& subNodes = adj.at(node);

                segHubCount += subNodes.size() > 1;

                int simpleSubNodes = subNodes.size() - hubCount[node - 1];
                combs = combs * fact(simpleSubNodes, modulo) % modulo;
                    
                for(int subNode : subNodes)
                {
                    if(subNode != from)
                    {
                        if(visitted[subNode - 1])
                        {
                            output << 0;
                            return 0;
                        }

                        queue.push(QueueEntry{ .to = subNode, .from = node });
                    }
                }
            }
        }

        if(segHubCount > 1) 
        { 
            combs = combs * 2 % modulo; 
        }  
    }

    combs = combs * fact(segCount, modulo) % modulo;
    combs = combs * exp % modulo;

    int nonFreeBirdCount = birdCount - freeBirdCount;

    for(int i = 0; i < freeBirdCount; ++i)
    {
        combs = combs * (nonFreeBirdCount + 2 + i) % modulo;
    }

    output << combs;

    return 0;
}