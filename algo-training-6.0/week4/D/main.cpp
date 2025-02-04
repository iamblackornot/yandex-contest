#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>

struct Node;
typedef Node* NodePtr;

struct Node
{
    int value;
    NodePtr left;
    NodePtr right;
};

class BinaryTree
{

public:
    bool Add(int value)
    {
        bool res = false;
        root = AddDFS(root, value, res);

        return res;
    }

    bool Search(int value)
    {
        NodePtr currNode = root;

        while(currNode)
        {
            if(currNode->value == value) return true;

            currNode = value < currNode->value ? currNode->left : currNode->right;
        }

        return false;
    }

    void PrintTree()
    {
        PrintTreeDFS(root, 0);
    }

    ~BinaryTree()
    {
        destroyDFS(root);
    }
private:

    void PrintTreeDFS(NodePtr node, int depth) 
    {
        if (!node) return;

        PrintTreeDFS(node->left, depth + 1);

        std::cout << std::string(depth, '.') << node->value << std::endl;

        PrintTreeDFS(node->right, depth + 1);
    }

    NodePtr AddDFS(NodePtr node, int value, bool& res)
    {
        if(!node)
        {
            res = true;
            return new Node{ .value = value };
        }

        if(value < node->value) 
            node->left = AddDFS(node->left, value, res);
        else if(value > node->value)
            node->right = AddDFS(node->right, value, res);
        else
            res = false;

        return node;
    }

    void destroyDFS(NodePtr node)
    {
        if(!node) return;

        destroyDFS(node->left);
        destroyDFS(node->right);

        delete node;
    }
private:
    NodePtr root = nullptr;
};

int main()
{
    BinaryTree btree;
    std::ifstream input("input.txt");
    std::string query;

    while(std::getline(input, query))
    {
        std::stringstream ss(query);

        std::string command;
        int value;

        ss >> command;
        ss >> value;

        if(command == "ADD") 
        {
            bool res = btree.Add(value);
            
            std::cout << (res ? "DONE" : "ALREADY") << std::endl;
        } 
        else if(command == "SEARCH")
        {
            bool res = btree.Search(value);
            
            std::cout << (res ? "YES" : "NO") << std::endl;
        }
        else if(command == "PRINTTREE")
        {
            btree.PrintTree();
        }
    }

    return 0;
}