#include <iostream>
#include <cmath>
#include <cassert>

namespace restricted {

template<class T>
class storage {

public:

    typedef std::vector<T> sequence;

    storage() { }

    storage(sequence& s): data(&s), 
                          block_size(log2(s.size()) / 2 > 2 ? log2(s.size()) / 2 : 2), 
                          pttrns(std::vector<block>(pow(2, block_size - 1))) {
        s.resize(s.size() + s.size() % block_size, s.back());
        raw = dummy_sequence(s.size() / block_size);
        dummy::storage<dummy_t>::fill(raw.size());
        build_pttrns();
        dummy = dummy::storage<dummy_t>(raw);
    }

    inline std::pair<T, size_t> rmq(size_t l, size_t r) const {
        size_t l_blocked = (l - 1 - ((l - 1) % block_size)) / block_size + 1;
        size_t r_blocked = (r + 1 - ((r + 1) % block_size)) / block_size;
        std::pair<T, size_t> min = dummy.rmq(l_blocked, r_blocked);
        if (l != l_blocked * block_size)
             min = std::min(min, dummy_rmq(l, l_blocked * block_size));
        if (r_blocked * block_size - 1 != r)
            min = std::min(min, dummy_rmq(r_blocked * block_size - 1, r));
        return min;
    }

    friend
    std::ostream& operator<<(std::ostream& os, const storage<T>& s) {
        os << "restricted: " << std::endl;
        os << "\tblock size: " << s.block_size << std::endl;
        os << "\traw: " << s.raw << std::endl;
        os << "\tpttrns: " << s.pttrns << std::endl;
        os << s.dummy; 
        return os;
    }

private:

    struct block {
        block(): offset(-1) { }
        int offset;

        friend 
        std::ostream& operator<<(std::ostream& os, const block& b) {
            os << b.offset;
            return os;
        }
    };

    typedef std::vector<bool> bits;
    typedef std::vector<size_t> positions;

    typedef std::pair<T, size_t> dummy_t;
    typedef std::vector<dummy_t> dummy_sequence;
    dummy::storage<dummy_t> dummy;

    sequence* data;

    dummy_sequence raw; 

    size_t block_size;
    std::vector<block> pttrns;

    inline int getHash(const bits& mask) {
        int index = 0;
        for (typename bits::const_iterator it = mask.begin(); 
             it != mask.end(); 
             ++it) {
            index <<= 1;
            index += *it;
        }
        return index;
    }

    inline bits getMask(const typename sequence::const_iterator& begin) {
        bits mask = bits(block_size - 1);
        for (typename sequence::const_iterator it = begin; 
             it != begin + block_size; 
             ++it) {
            if (*(it + 1) > *it) {
                mask[it - begin] = true;
            } else {
                mask[it - begin] = false;
            } 
        }
        return mask;
    }

    void build_pttrns() {
        bits mask;
        int hash = 0;
        for (typename sequence::const_iterator it = data->begin();
             it != data->end();
             it += block_size) {
            mask = getMask(it);
            hash = getHash(mask);
            if (pttrns[hash].offset == -1)
                buildNewPttrn(pttrns[hash].offset, mask, it);
            raw[(it - data->begin()) / block_size] = std::make_pair(*(it + pttrns[hash].offset), (it - data->begin()) + pttrns[hash].offset);
        }
    }

    inline void buildNewPttrn(int& ofs, const bits& mask, const typename sequence::const_iterator& iterator) {
        int value = 0, min = 0;
        ofs = 0;
        for (typename bits::const_iterator it = mask.begin();
                it != mask.end();
                ++it) {
            if (*it == false) {
                value--;
                min = std::min(min, value);
                if (min == value) 
                    ofs = it - mask.begin() + 1;
            } else {
                value++;
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
