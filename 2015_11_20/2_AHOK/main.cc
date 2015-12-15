#include <iostream>
#include <vector>
#include <string>

void prefix(std::vector<int>& pi, const std::string& s) {
    pi.resize(s.length());
    for (int i = 1; i < s.length(); ++i) {
        int j = pi.at(i-1);
        while (j > 0 && s[i] != s[j])
            j = pi.at(j-1);
        if (s[i] == s[j])
            ++j;
        pi.at(i) = j;
    }
}

std::vector<int> prefix(const std::string& s) {
    std::vector<int> pi(s.length());
    prefix(pi, s);
    return pi;
}

template<class T>
std::ostream& operator <<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    if (v.empty()) {
        os << " ] 0";
        return os;
    }
    for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end() - 1; ++it) {
        os << *it << ", ";
    }
    os << *(--v.end()) << "] " << v.size();
    return os;
}

int period(const std::string& string) {
    int period = -1;
    std::vector<int> pi(string.length());
    prefix(pi, string + string);
    for(typename std::vector<int>::const_iterator it = pi.begin() + string.length(); it != pi.end(); ++it) {
        if (*it >= string.length()) {
            period = (it - pi.begin()) - string.length() + 1;
            break;
        }
    }
    return period;
}

int main() {
    size_t qty = 0;
    std::cin >> qty;
    for (int i = 0; i < qty; ++i) {
        std::string string;
        std::cin >> string;
        std::cout << period(string) << std::endl;
    }
    return 0;
}
