#include <iostream>
#include <fstream>

#include "finder.hpp"

int main() {
    finder f;
    std::string filepath;
    std::getline(std::cin, filepath);
    size_t q = 0;
    std::cin >> q;
    std::string s;
    std::getline(std::cin, s);
    for (int i = 0; i < q; i++) {
        std::getline(std::cin, s);
        f.addPattern(s);
    }
    std::ifstream file(filepath);
    std::string str;
    std::string file_contents;
    while (std::getline(file, str)) {
        file_contents += str;
        file_contents.push_back('\n');
    }
    auto set = f.findAll(file_contents);   
    for (auto it: set) {
        std::cout << it << std::endl;
    }
}
