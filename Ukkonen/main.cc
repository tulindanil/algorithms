#include <iostream>
#include <cassert>
#include <string>
#include <map>
#include <vector>
#include "SuffixTree.h"

int main() {
    std::string s1 = "I-love-basketball";
    std::string s2 = "I-love-football";
    SuffixTree tree(s1  + std::string("#") + s2);
    std::cout << s1 << std::endl << s2 << std::endl;
    tree.solve(std::cout, s1);
    return 0;
}
