#include <vector>
#include <fstream>
#include <unordered_map>
#include <iostream>

using namespace std;

int dfs(unordered_map<int, vector<int>>& edges, string& leaves, int k, int node, int depth, bool max, bool reverse) {

    if(leaves[node - 1] != '.') {
        if(!reverse) {
            if(leaves[node - 1] == '+') { return 1; }
            if(leaves[node - 1] == '0') { return 0; }
            if(leaves[node - 1] == '-') { return -1; }
        } else {
            if(leaves[node - 1] == '+') { return -1; }
            if(leaves[node - 1] == '0') { return 0; }
            if(leaves[node - 1] == '-') { return 1; }
        }
    }

    auto& nodes = edges[node];
    if(max) {
        int res = -1;
        for(int n : nodes) {
            res = std::max(res, dfs(edges, leaves, k, n, depth + 1, !max, reverse));
            if(res == 1) { return res; }
        }
        if(depth == k && !reverse) {      
            for(int n : nodes) {
                res = std::max(res, dfs(edges, leaves, k, n, depth + 1, !max, true));
                if(res == 1) { return res; }
            }
        }
        return res;
    } else {
        int res = 1;
        for(int n : nodes) {
            if(leaves[n - 1] == '.'){
                res = std::min(res, dfs(edges, leaves, k, n, depth + 1, !max, reverse));
            } else {
                res = std::min(res, dfs(edges, leaves, k, n, depth + 1, !max, !reverse));
            }
            if(res == -1) { return res; }
        }
        if(depth == k && !reverse) {      
            for(int n : nodes) {
                //res = std::min(res, dfs(edges, leaves, k, n, depth + 1, !max, true));
                if(leaves[n - 1] == '.'){
                    res = std::min(res, dfs(edges, leaves, k, n, depth + 1, !max, true));
                } else {
                    res = std::min(res, dfs(edges, leaves, k, n, depth + 1, !max, false));
                }
                if(res == -1) { return res; }
            }
        }
        return res;
    }

}

int main() {
    std::ifstream inputFile("input.txt");

    int n, k;
    string leaves;
    inputFile >> n >> k >> leaves;
    unordered_map<int, vector<int>> edges;

    for(int i = 0; i < n - 1; i++) {
        int first, second;
        inputFile >> first >> second;
        edges[first].push_back(second);
    }

    int res = dfs(edges, leaves, k, 1, 0, true, false);

    if(res == -1) {
        cout << "Second";
    } else if(res == 0) {
        cout << "Draw";
    } else {
        cout << "First";
    }

    return 0;
}
