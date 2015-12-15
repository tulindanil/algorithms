#include <iostream>
#include <map>

#ifdef DEBUG

template<class K, class T>
std::ostream& operator <<(std::ostream& os, const std::pair<K, T>& p) {
    os << "{" << p.first << ", " << p.second << "}";
    return os;
}

template<class T>
std::ostream& operator <<(std::ostream& os, const std::vector<T>& v) {
    if (v.empty()) return os;
    os << "[";
    for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
        os << *it << (((it - v.begin()) != v.size() - 1) ? ", " : "]"); 
    }
    os << " " << v.size();
    return os;
}

template<class T, class K>
std::ostream& operator <<(std::ostream& os, const std::map<T, K>& map) {
    os << "[";
    for (typename std::map<T, K>::const_iterator it = map.begin(); it != --map.end(); ++it) {
        os << "(" << it->first << ", " << it->second << "), ";
    }
    os << "(" << (--map.end())->first << ", " << (--map.end())->second << ")" << "]";
    return os;
}

#endif
 
#include "solver.cc"

template<typename Queue>
void wrap() {
    Solver<Queue>::solve(std::cin, std::cout);     
}
