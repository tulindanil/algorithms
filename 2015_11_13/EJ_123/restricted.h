#include <cmath>
#include <cassert>
#include "dummy_storage.h"

namespace restricted {

template<class T>
class storage {

public:

    storage(sequence& s): data(&s), block_size(log2(s.size) / 2) {
        s.resize(s.size() + s.size() % block_size);
        dummy::storage::hold(s.size());
    }

    inline T rmq(size_t l, size_t r) const {
        size_t l_blocked = (l + block_size - l % block_size) / block_size;
        size_t r_blocked = (r - r % block_size) / block_size;
        dummy_t block_max = dummy.rmq(l_blocked, r_blocked);
        T min = dummy_rmq(l, l + block_size - l % block_size);
        min = std::min(min, dummy_rmq(r_blocked, r);
        return min;
    }

private:

    typedef std::vector<T> sequence;
    typedef std::vector<bool> bits;

    typedef std::pair<T, size_t> dummy_t;
    dummy::storage<dummy_t> dummy;

    sequence* data;
    size_t block_size;


    inline T dummy_rmq(size_t l, size_t r) const {
        assert(data != NULL);
        T min = T();
        for (typename sequence::const_iterator it = data->begin() + l; it != data->begin() + r; ++it) {
            min = std::min(min, *it);
        }
        return min;
    }


};

};
