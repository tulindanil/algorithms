#include <iostream>
#include <cassert>
#include <string>
#include <map>
#include "SuffixTree.h"

int main() {
    int k;
    std::string word;
    std::cin >> k >> word;
    int length = word.length();
    for (int start = 0; start < length; ++start) {
        SuffixTree tree;
        for (int i = 0; i < k; ++i)
            tree.appendChar(word[(start + i) % length]);
        if (start)
            std::cout << ' ';
        std::cout << tree.substringsCount();
    }
    std::cout << '\n';
}
