#include <iostream>
#include <vector>

#ifdef DEBUG

template<class V, class K>
std::ostream& operator<<(std::ostream& os, const std::pair<K, V>& p) {
    os << "{" << p.first << ", " << p.second << "}";
    return os;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    if (v.size() == 0)
        return os;
    os << "[";
    for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
        bool isLast = (it == v.end() - 1);
        os << *it << (isLast ? "]" : ", "); 
    }
    os << " " << v.size();
    return os;
}

#endif 

#include "dummy_tree.h"

void read(dummy::tree &t) {
    std::vector<dummy::node::ref> v(t.size, NULL);
    v[0] = t.root;
    int anc, chd;
    for (int i = 0; i < t.size - 1; ++i) {
        scanf("%d %d", &chd, &anc);
        if (v[chd] == NULL)
            v[chd] = new dummy::node(chd);
        if (v[anc]->l == NULL)
            v[anc]->l = v[chd];
        else if (v[anc]->r == NULL)
            v[anc]->r = v[chd];
   }
}

void work() { 
    size_t q = 0;
    std::cin >> q;
    dummy::tree t(0, q + 1);
    read(t);
    t.hold();
#ifdef DEBUG
    std::cout << t << std::endl;
#endif
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
