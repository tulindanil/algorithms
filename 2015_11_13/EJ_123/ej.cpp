#include <iostream>
#include <stack>
#include <vector>
#include <cassert>
#include <vector>
#include <cmath>

namespace dummy {

using std::pow;

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

    storage() {

    }

    storage(const std::vector<T> &v): m(matrix(_log(v.size()) + 1, row(v.size(), T()))) {
        m.at(0) = v;
        for (size_t k = 1; k <= _log(v.size()); ++k) {
            for (int i = 0; i <= (int)v.size() - pow(k); ++i) {
                try {
                    m.at(k).at(i) = std::min(m.at(k - 1).at(i), m.at(k - 1).at(i + pow(k - 1)));
                } catch (std::exception &) {
                    std::cout << v.size() - pow(2, k) << " " << v.size() - dummy::pow(k)  << std::endl;
                    abort();
                }
            }
        }
    }

    T rmq(size_t begin, size_t end) const {
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

        bins = new long long int[size];
        for (size_t i = 0; i < size; ++i) {
            bins[i] = pow(2, i);
        }
    }

private:

    typedef std::vector<T> row;
    typedef std::vector<row> matrix;

    matrix m;

};
}

namespace dummy {

struct node {

    typedef node* ref;

    int key;

    node(const int& k): key(k), r(NULL), l(NULL) { }

    ref r;
    ref l;
};

struct tree {

    node::ref root;

    typedef std::pair<int, int> rmq_type;
    typedef std::vector<rmq_type> ordered;

    size_t size;

    tree() { }
    tree(const int& r, const size_t& s): root(new node(r)), size(s) { }

    ordered o;
    std::vector<int> f;

    storage<rmq_type> s;

    void dfsvisit(node::ref n, int depth = 0) {
        rmq_type pair = std::make_pair(depth, n->key);
        o.push_back(pair);

        if (n->r != NULL) {
            dfsvisit(n->r, depth + 1);
            o.push_back(pair);
        }

        if (n->l != NULL) {
            dfsvisit(n->l, depth + 1);
            o.push_back(pair);
        }

        delete n;
    }

    void dfs() {
        dfsvisit(root);
    }

    void hold() {
        dfs();
        f = std::vector<int>(size, -1);
        for (ordered::const_iterator it = o.begin(); it != o.end(); ++it) 
            if (f[it->second] == -1)
                f[it->second] = it - o.begin();
        storage<rmq_type>::fill(o.size());
        s = storage<rmq_type>(o);
    }

    int lca(const int& left, const int& right) const {
        int l = std::min(f[left], f[right]);
        int r = std::max(f[left], f[right]);
        return s.rmq(l, r).second;
    }

};

}

using dummy::tree;
using dummy::node;

void read(tree &t) {
    std::vector<node::ref> v(t.size, NULL);
    v[0] = t.root;
    int anc, chd;
    for (int i = 0; i < t.size - 1; ++i) {
        scanf("%d %d", &chd, &anc);
        if (v[anc] == NULL) {
            std::cout << "v[anc] == NULL" << std::endl;
            abort();
        }
        if (v[chd] == NULL)
            v[chd] = new node(chd);

        if (v[anc]->l == NULL)
            v[anc]->l = v[chd];
        else if (v[anc]->r == NULL)
            v[anc]->r = v[chd];
        else {
            std::cout << chd << " " << anc << std::endl;
            std::cout << "NOTNULL" << std::endl;
            abort();
        }
    }
}

void work() { 
    size_t q = 0;
    std::cin >> q;
    tree t(0, q + 1);
    read(t);
    t.hold();
    std::cin >> q;
    int fst, snd;
    for (int i = 0; i < q; i++) {
        scanf("%d %d", &fst, &snd);
        printf("%d\n", t.lca(fst, snd));
    }
}

int main() {
    work();
    return 0;
}
