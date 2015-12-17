#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

namespace dummy {

using std::pow;

char *logs;
long long int *bins;

inline int _log(int x) {
   return logs[x];
}

inline int log(int x) {
   return logs[x];
}

inline int powered(int x) {
    size_t value = 1;
    while (x > 0) {
        value <<= 1;
        x--;
    }
    return value;
}

int pow(int x) {
    return powered(x);
}

template<class T>
class storage {

public:

    storage() {

    }

    storage(std::vector<T> &v): data(&v), m(matrix(_log(v.size()) + 1)) {
        size_t size = v.size();
        m.at(0) = v;
        for (size_t k = 1; k <= _log(size); ++k) {
            m.at(k) = row((int)size - pow(k - 1) + 1);
            for (int i = 0; i <= (int)size - pow(k); ++i) {
                m.at(k).at(i) = std::min(at(k - 1, i), at(k - 1, i + pow(k - 1)));
            }
        }
    }

    inline const T& at(size_t row, size_t column) const {
        try {
            //if (row == 0) {
//                if (data == NULL)
//                    std::cout << "data is nil" << std::endl, abort();
            //    return data->at(column);
           // }
            return m.at(row).at(column);
        } catch (std::exception& e) {
            abort();
        }
    }

    inline T rmq(size_t begin, size_t end) const {
        size_t length = end - begin;
        int k = _log(length);
        return std::min(at(k, begin), at(k, end - dummy::pow(k)));
    }

    static void fill(size_t size) {
        logs = new char[size + 1]; 
        int pos = 0;
        int lim = 2;
        int log = 0;

        while(pos < size + 1) {
            while(pos < lim) {
                logs[pos] = log;
                pos++;
            }

            lim <<= 1;
            log++;

            if (lim > size + 1)
                lim = size + 1;
        }
   }

    const T& operator[](size_t index) {
        return m[0][index];
    }

#ifdef DEBUG
    template<class V> 
    friend std::ostream& operator<<(std::ostream& os, const storage<V>& s) {
        os << "dummy::storage" << std::endl;
//        os << *(s.data) << std::endl;
        for (typename storage<V>::matrix::const_iterator it = s.m.begin(); it != s.m.end(); ++it) {
            os << *it << std::endl;
        }
        return os;
    }
#endif

private:

    typedef std::vector<T> row;
    typedef std::vector<row> matrix;

    matrix m;
    row *data;

};
}
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <vector>

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
        return dummy_rmq(l, r);
        if (r - l >= block_size) {
            int l_blocked = (l - 1 - ((l - 1) % block_size)) / block_size + 1;
            int r_blocked = (r + 1 - ((r + 1) % block_size)) / block_size;
            std::pair<T, size_t> min = dummy.rmq(l_blocked, r_blocked);
            if (l != l_blocked * block_size)
                 min = std::min(min, dummy_rmq(l, l_blocked * block_size));
            if (r_blocked * block_size - 1 != r)
                min = std::min(min, dummy_rmq(r_blocked * block_size - 1, r));
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
            min = std::min(min, std::make_pair(*it, (long)(it - (data->begin() + l))));
        }
        return min;
    }


};

};

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
