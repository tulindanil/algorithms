#include <iostream>
#include <cmath>
#include <cassert>

#include "dummy_storage.h"

int main() {

    size_t n = 1500000;

    dummy::storage<int>::fill(n);

    for (int i = 0; i < log2(n) + 1; ++i)
        assert(pow(2, i) ==  dummy::pow(i));

    for (int i = 1; i < n; ++i) {
       assert((int)log2(i)  == dummy::_log(i));
    }
}
