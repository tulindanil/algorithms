#include <iostream>
#include <cmath>
#include <cassert>

namespace restricted {

template<class T>
class storage {

public:

    typedef std::vector<T> sequence;

    storage() { }

    storage(sequence& s): data(&s), block_size(log2(s.size()) / 2), 
                          pttrns(std::vector<block>(pow(2, block_size - 1))) {
        s.resize(s.size() + s.size() % block_size);
        dummy_sequence raw = dummy_sequence(s.size() / block_size);
        dummy::storage<dummy_t>::fill(raw.size());
        build_pttrns(raw);
        dummy = dummy::storage<dummy_t>(raw);
    }

    inline std::pair<T, size_t> rmq(size_t l, size_t r) const {
        size_t l_blocked = (l + block_size - l % block_size) / block_size;
        size_t r_blocked = (r - r % block_size) / block_size;
        std::pair<T, size_t> block_min = dummy.rmq(l_blocked, r_blocked),
        min = std::min(block_min, dummy_rmq(l, l + block_size - l % block_size));
        min = std::min(min, dummy_rmq(r_blocked, r));
        return min;
    }

private:

    struct block {
        block(): offset(-1) { }
        int offset;
    };

    typedef std::vector<bool> bits;
    typedef std::vector<size_t> positions;

    typedef std::pair<T, size_t> dummy_t;
    typedef std::vector<dummy_t> dummy_sequence;
    dummy::storage<dummy_t> dummy;

    sequence* data;
    sequence blocked_data;

    size_t block_size;
    std::vector<block> pttrns;

    inline int getHash(const bits& mask) {
        int index = 0;
        for (typename bits::const_iterator it = mask.begin(); 
             it != mask.end(); 
             ++it) {
            index <<= *it;
        }
        return index;
    }

    inline bits getMask(const typename sequence::const_iterator& begin) {
        bits mask = bits(block_size - 1);
        for (typename sequence::const_iterator it = begin; 
             it != begin + block_size - 1; 
             ++it) {
            if (*(it + 1) > *it) {
                mask[it - begin] = true;
            } else {
                mask[it - begin] = false;
            } 
        }
        return mask;
    }

    void build_pttrns(dummy_sequence& s) {
        bits mask;
        int hash = 0;
        for (typename sequence::const_iterator it = data->begin();
             it != data->end();
             it += block_size) {
            mask = getMask(it);
            hash = getHash(mask);
            if (pttrns[hash].offset == -1)
                buildNewPttrn(pttrns[hash].offset, mask, it);
            s[(it - data->begin()) / block_size] = std::make_pair(*(it + pttrns[hash].offset), (it - data->begin()) + pttrns[hash].offset);
        }
    }

    inline void buildNewPttrn(int& ofs, const bits& mask, const typename sequence::const_iterator& iterator) {
        T min = *iterator;
        ofs = 0;
        for (typename bits::const_iterator it = mask.begin();
                it != mask.end();
                ++it) {
            if (*it == false) {
                min = std::min(min, *(iterator + (it - mask.begin())));
                if (*(iterator + (it - mask.begin())) == min) 
                    ofs = it - mask.begin();
            }
        }
    }

    inline std::pair<T, size_t> dummy_rmq(size_t l, size_t r) const {
        assert(data != NULL);
        std::pair<T, long> min = std::make_pair(data->at(l), l);
        for (typename sequence::const_iterator it = data->begin() + l; it != data->begin() + r; ++it) {
            min = std::min(min, std::make_pair(*it, it - (data->begin() + l)));
        }
        return min;
    }


};

};
