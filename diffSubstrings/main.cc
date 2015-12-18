#include <iostream>
#include <cassert>
#include <string>
#include <map>
#include <vector>
#include <list>

struct Result {

    size_t current;
    size_t length;
    size_t matching;

    friend std::ostream& operator <<(std::ostream& os, const Result& r) {
        os << r.current << " | " << r.length << " | " << r.matching;
        return os;
    }

};

#include "SuffixTree.h"

int main() {
    std::string s;
    std::cin >> s;
    SuffixTree tree(s);
    std::cout << tree.count() << std::endl;
    return 0;
}
