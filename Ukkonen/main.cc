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
    std::string s1 = "I-love-basketball";
    std::string s2 = "I-love-football";
    std::cout << s1 << std::endl << s2 << std::endl;

    std::reverse(s1.begin(), s1.end());
    std::reverse(s2.begin(), s2.end());

    SuffixTree tree(s2);
    tree.proceed(s1);
    return 0;
}
