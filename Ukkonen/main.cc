#include <iostream>
#include <cassert>
#include <string>
#include <map>
#include "SuffixTree.h"

int main() {
    int k;
    std::string word;
    std::cin >> word;
    int length = word.length();
    SuffixTree tree;
    for (auto it: word) 
        tree.push_back(it);
    std::cout << tree.count() << std::endl;
}
