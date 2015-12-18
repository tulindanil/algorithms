#include <iostream>
#include <vector>

#ifdef DEBUG

template<class V, class K>
std::ostream& operator<<(std::ostream& os, const std::pair<K, V>& p) {
    os << "{" << p.first << ", " << p.second << "}";
    return os;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    if (v.size() == 0)
        return os;
    os << "[";
    for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
        bool isLast = (it == v.end() - 1);
        os << *it << (isLast ? "]" : ", "); 
    }
    os << " " << v.size();
    return os;
}

#endif 

#include "dummy_storage.h"
#include "restricted.h"

using restricted::storage;

int main() {

    std::cin.sync_with_stdio();
    std::cout.sync_with_stdio();

    size_t size = 0;
    std::cin >> size;
    std::vector<float> v(size, 0);

    for (size_t i = 0; i < size; ++i) {
        float value;
        std::cin >> value;
        v.at(i) = value;
    }

    storage<float> s = storage<float>(v);

#ifdef DEBUG
    std::cout << s << std::endl;
#endif 

    std::cin >> size;
    int fst, snd;
    for (size_t i = 0; i < size; ++i) {
        //std::cin >> fst >> snd;
        scanf("%d %d", &fst, &snd);
        printf("%f\n", s.rmq(fst, snd).first);
        //std::cout << s.rmq(fst, snd) << std::endl;
    }

    return 0;
}
