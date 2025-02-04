#include <cstdint>
#include <fstream>
#include <unordered_map>
#include <vector>

typedef std::vector<int> NodeList;
typedef std::unordered_map<int, NodeList> AdjacencyMap;

struct NodeStats
{
    int subTreeSize = 0;
    uint64_t subTreeValue = 0;
};


NodeStats DFS(
    AdjacencyMap const& adj, 
    std::vector<uint64_t>& nodeValues, 
    int currNode)
{
    NodeStats res { .subTreeSize = 1, .subTreeValue = 0 };

    if(adj.contains(currNode))
    {
        NodeList const& nodes = adj.at(currNode);

        for(int i = 0; i < nodes.size(); ++i)
        {
            NodeStats stats = DFS(adj, nodeValues, nodes[i]);

            res.subTreeSize += stats.subTreeSize;
            res.subTreeValue += stats.subTreeValue;
        }
    }

    res.subTreeValue += res.subTreeSize;
    nodeValues[currNode - 1] = res.subTreeValue;

    return res;
}

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int count;
    input >> count;

    AdjacencyMap adj;
    std::vector<uint64_t> nodeValues(count, 0);

    for(int i = 1; i < count; ++i)
    {
        int boss;
        input >> boss;

        adj[boss].push_back(i + 1);
    }

    DFS(adj, nodeValues, 1);
    
    for(int i = 0; i < count; ++i)
    {
        if(i != 0) { output << " "; }
        
        output << nodeValues[i];
    }

    return 0;
}