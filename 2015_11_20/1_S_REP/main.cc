#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <streambuf>
#include <map>
#include <memory>

template<class T, class K>
std::ostream& operator <<(std::ostream& os, const std::map<T, K>& m) {
    os << "map:" << std::endl;
    for (auto it: m) {
        os << it.first << "-" << it.second << std::endl;
    }
    return os;
}

template<class T>
std::ostream& operator <<(std::ostream& os, const std::vector<T>& v) {
    os << "vector:" << std::endl;
    for (auto it = v.begin(); it != v.end(); ++it) {
        os << it - v.begin() << "-" << *it << " ";
    }
    return os;
}

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
