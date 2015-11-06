#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

#define min(x, y) (x) > (y)? (y) : (x)

using std::vector;

char *logs;
long long int *bins;

int _log(int x) {
    return logs[x] + 1;
}

int powered(int x) {
    size_t value = 1;

    while (x > 0) {
        value <<= 1;
        x--;
    }

    return value;
}

int pow(int x) {
    return bins[x];
}

template<class T>
class storage {

public:

    storage(const vector<T> &v): m(matrix(_log(v.size()) + 1, row(v.size(), T()))) {
        m.at(0) = v;

        for (size_t k = 1; k <= _log(v.size()); ++k) {
            for (int i = 0; i <= (int)v.size() - pow(k); ++i) {
                try {
                    m.at(k).at(i) = min(m.at(k - 1).at(i), m.at(k - 1).at(i + pow(k - 1)));
                } catch (std::exception &) {
                    std::cout << v.size() - pow(2, k) << " " << v.size() - pow(k)  << std::endl;
                    abort();
                }
            }
        }
    }

    T rmq(size_t begin, size_t end) const {
        size_t length = end - begin;
        int k = _log(length) - 1;
        return min(m.at(k).at(begin), m.at(k).at(end - pow(k)));
    }

private:

    typedef vector<T> row;
    typedef vector<row> matrix;

    matrix m;

};

int main() {

    size_t size = 0;
    scanf("%lu", &size);
    vector<float> v(size, 0);

    logs = new char[size + 1]; 
    bins = new long long int[size];

    for (size_t i = 0; i < size; ++i) {
        bins[i] = pow(2, i);

        float value;
        scanf("%f", &value);
        v.at(i) = value;
    }

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

    storage<float> s = storage<float>(v);

    std::cin >> size;
    size_t fst, snd;
    for (size_t i = 0; i < size; ++i) {
        scanf("%lu %lu", &fst, &snd);

        printf("%.6f\n", s.rmq(fst, snd));
    }

    return 0;
}
