#include <vector>
#include "dummy_storage.h"
#include "restricted.h"

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

    restricted::storage<rmq_type> s;

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
        s = restricted::storage<rmq_type>(o);
    }

    inline int lca(const int& left, const int& right) const {
        int l = std::min(f[left], f[right]);
        int r = std::max(f[left], f[right]);
        return s.rmq(l, r).first.second;
    }

#ifdef DEBUG
    friend std::ostream& operator <<(std::ostream& os, const tree& t) {
        std::vector<int> vertecies;
        for (std::vector<std::pair<int, int> >::const_iterator it = t.o.begin(); it != t.o.end(); ++it)
            vertecies.push_back(it->second);
        os << "tree:" << std::endl;
        os << "\torder :";
        os << vertecies << std::endl;
        os << "\tfirst :";
        os << t.f << std::endl;
        os << t.s;
        return os;
    }
#endif

};

}
