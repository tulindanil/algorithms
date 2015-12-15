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
