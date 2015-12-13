#include <iostream>
#include "dummy_storage.h"

using dummy::storage;

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

    storage<float>::fill(v.size());
    storage<float> s = storage<float>(v);

    std::cin >> size;
    size_t fst, snd;
    for (size_t i = 0; i < size; ++i) {
        std::cin >> fst >> snd;
        std::cout << s.rmq(fst, snd) << std::endl;
    }

    return 0;
}
