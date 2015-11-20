#include <iostream>
#include <string>
#include <map>
#include <set>
#include <cassert>
#include <algorithm>
#include <vector>

std::ostream& operator <<(std::ostream& os, const std::vector<int> &v) {
    for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it)
        os << *it << " ";
    os << std::endl;
    return os;
}

#include "MaxFlow.cpp"

template<class W>
class worker {
    
public:

    static void solve(std::istream &is,
                      std::ostream& os) {
        worker<W> w;
        w.run(is, os); 
    }

private:

    typedef int T;
    typedef std::vector<W> row;
    typedef std::vector<row> matrix;

    matrix capacity;
   
    void run(std::istream &is, 
             std::ostream &os) {
        read(is);
        T src = 0, trg = capacity.size() - 1;
        os << flow<W>::getMaxFlow(capacity, src, trg) << std::endl;
    }

    void read(std::istream &is) {
        size_t s;
        is >> s;
        size_t edges_qty;
        is >> edges_qty;
        capacity = matrix(s, row(s));
        T src, dst; 
        W weight;
        for (size_t index = 0; index < edges_qty; ++index) {
            is >> src >> dst >> weight;
            if (src == dst)
                continue;
            src--;
            dst--;
            capacity[src][dst] += weight;
        }
    }
};

template<class W>
void work() {
    worker<W>::solve(std::cin,
                     std::cout);
}

int main() {
    work<long long int>();
    return 0;
}
