#include <iostream>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <vector>

class Debug {

public:

    template<class T>
    friend std::ostream& operator <<(Debug& d, const T& object) {
#ifdef DEBUG
        std::cout << object;
#endif
        return std::cout;
    }

};

Debug debug;

namespace restricted {

template<class T>
class storage {

public:

    typedef std::vector<T> sequence;

    storage() { }

    storage(sequence& s): data(s), 
                          block_size(log2(s.size()) / 2 > 2 ? log2(s.size()) / 2 : 2), 
                          pttrns(std::vector<block>(pow(2, block_size - 1))) {
        data.resize(s.size() + ((s.size() % block_size) ? (block_size - s.size() % block_size) : 0), s.back());
        raw = dummy_sequence((s.size() / block_size) + 1);
        dummy::storage<dummy_t>::fill(raw.size());
        build_pttrns();
        dummy = dummy::storage<dummy_t>(raw);
    }

    inline std::pair<T, size_t> rmq(int l, int r) const {
#ifdef DEBUG
        debug << "rmq query " << l << " " << r << std::endl; 
#endif
        if (r - l > block_size) {
            int l_blocked = 0;
            if (l == 0)
                l_blocked = 0;
            else
                l_blocked = (l - 1 - ((l - 1) % block_size)) / block_size + 1;
            int r_blocked = (r - ((r) % block_size)) / block_size;
#ifdef DEBUG
            debug << "l_blocked: " << l_blocked << std::endl;
            debug << "r_blocked: " << r_blocked << std::endl;
#endif
            std::pair<T, size_t> min = dummy.rmq(l_blocked, r_blocked);
            if (l != l_blocked * block_size)
                 min = std::min(min, dummy_rmq(l, l_blocked * block_size));
            if (r_blocked * block_size - 1 != r)
                min = std::min(min, dummy_rmq(r_blocked * block_size - 1, r));
#ifdef DEBUG
            debug << "min is " << min << std::endl;
#endif
            return min;
        } else {
            return dummy_rmq(l, r);
        }
    }

#ifdef DEBUG
    friend std::ostream& operator<<(std::ostream& os, const storage<T>& s) {
        os << "restricted: " << std::endl;
        os << "\tblock size: " << s.block_size << std::endl;
        os << "\traw: " << s.raw << std::endl;
        os << "\tpttrns: " << s.pttrns << std::endl;
        os << s.dummy; 
        return os;
    }
#endif

private:

    struct block {
        block(): offset(-1) { }
        int offset;

#ifdef DEBUG
        friend std::ostream& operator<<(std::ostream& os, const block& b) {
            os << "block " << b.offset;
            return os;
        }
#endif
    };

    typedef std::vector<bool> bits;
    typedef std::vector<size_t> positions;

    typedef std::pair<T, size_t> dummy_t;
    typedef std::vector<dummy_t> dummy_sequence;
    dummy::storage<dummy_t> dummy;

    sequence data;

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
        for (typename sequence::const_iterator it = data.begin();
            it != data.end();
            it += block_size) {
            mask = getMask(it);
            hash = getHash(mask);
            if (pttrns[hash].offset == -1)
                buildNewPttrn(pttrns[hash].offset, mask, it);
            try {
                raw.at((it - data.begin()) / block_size) = std::make_pair(*(it + pttrns[hash].offset), (it - data.begin()) + pttrns[hash].offset);
            } catch (std::exception& e) {
#ifdef DEBUG
                std::cout << *this << std::endl;
                std::cout << "data size: " << data.size() << std::endl; 
                std::cout << "exception at raw " << (it - data.begin()) / block_size << std::endl;
#endif 
                abort();
            }
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
        std::pair<T, long> min = std::make_pair(data.at(l), l);
        for (typename sequence::const_iterator it = data.begin() + l; it != data.begin() + r; ++it) {
            min = std::min(min, std::make_pair(*it, (long)(it - (data.begin() + l))));
        }
        return min;
    }


};

};
