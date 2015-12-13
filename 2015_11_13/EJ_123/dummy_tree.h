#include <vector>
#include "dummy_storage.h"

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

        if (n->l != NULL) {
            dfsvisit(n->l, depth + 1);
            o.push_back(pair);
        }

        if (n->r != NULL) {
            dfsvisit(n->r, depth + 1);
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

    inline int lca(const int& left, const int& right) const {
        int l = std::min(f[left], f[right]);
        int r = std::max(f[left], f[right]);
        return s.rmq(l, r + 1).second;
    }

};

}
