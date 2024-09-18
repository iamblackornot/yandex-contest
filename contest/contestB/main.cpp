#include <fstream>
#include <algorithm>
#include <iostream>
#include <set>

using namespace std;

int main() {

    std::ifstream input("input.txt");

    int n, x, k;
    input >> n >> x >> k;

    std::set<int> t;

    for(size_t i = 0; i < n; ++i)
    {
        int v;
        input >> v;
        t.insert(v);
    }

    while(k > 1)
    {
        // std::cout << "k=" << k;
        // std::for_each(t.begin(), t.end(), 
        //     [](auto const& el){ std::cout << " " << el; });
        
        // std::cout << std::endl << std::endl;

        int d = *t.begin();
        t.erase(t.begin());
        t.insert(d + x);
        --k;
    }

    std::ofstream output("output.txt");

    output << *t.begin() << endl;

    return 0;
}
