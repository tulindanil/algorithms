#include <iostream>
#include <stack>
#include "dummy_tree.h"

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
