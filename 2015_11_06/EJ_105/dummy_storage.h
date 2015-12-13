#include <cassert>
#include <vector>
#include <cmath>

namespace dummy {

using std::pow;

char *logs;
long long int *bins;

inline int _log(int x) {
    return logs[x] + 1;
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
//    assert(bins[x] == powered(x));
    return powered(x);
    //return bins[x];
}

template<class T>
class storage {

public:

    storage() {

    }

    storage(const std::vector<T> &v): m(matrix(_log(v.size()) + 1, row(v.size(), T()))) {
        m.at(0) = v;
        for (size_t k = 1; k <= _log(v.size()); ++k) {
            for (int i = 0; i <= (int)v.size() - pow(k); ++i) {
//                try {
                m.at(k).at(i) = std::min(m.at(k - 1).at(i), m.at(k - 1).at(i + pow(k - 1)));
/*                } catch (std::exception &) {
                    std::cout << v.size() - pow(2, k) << " " << v.size() - dummy::pow(k)  << std::endl;
                    abort();
                }
*/            }
        }
    }

    inline T rmq(size_t begin, size_t end) const {
        size_t length = end - begin;
        int k = _log(length) - 1;
        return std::min(m.at(k).at(begin), m.at(k).at(end - dummy::pow(k)));
    }

    static void fill(size_t size) {
        logs = new char[size + 1]; 
        int pos = 0;
        int lim = 2;
        int log = 0;

        while(pos < size) {
            while(pos < lim) {
                logs[pos] = log;
                pos++;
            }

            lim <<= 1;
            log++;

            if (lim > size)
                lim = size;
        }
        logs[size] = logs[size - 1];

 //       bins = new long long int[size];
 //       for (size_t i = 0; i < size; ++i) {
 //           bins[i] = pow(2, i);
 //       }
    }

private:

    typedef std::vector<T> row;
    typedef std::vector<row> matrix;

    matrix m;

};
}
