#include "treap.cc"
#include "testlib.cc"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <set>

#define sz(v) ((int)((v).size()))
#define forn(i,n) for (int i = 0; i < (n); i++)
#define forit(it,v) for (typeof((v).begin()) it = (v).begin(); it != (v).end(); ++it)

void test_simple_insert_count_erase() {
    treap<int> a;
    const int n = 10;

    forn(i, n) a.insert(i);
    assert_equals(sz(a), n);
    
    forn(i, n)
    {
        assert_true(a.count(i));
        a.insert(i);
        assert_true(a.count(i));
        assert_equals(sz(a), n);
    }
    
    for(int i = 1; i < n - 1; i++)
    {
        a.erase(i);
        assert_true(!a.count(i));
        assert_equals(sz(a), n - i);
    }
    
    a.insert(1); 
    assert_equals(sz(a), 3);

    a.erase(n-1);
    a.erase(0);
    assert_equals(sz(a), 1);
    assert_true(a.count(1));

    a.erase(1);
    assert_true(a.empty());
}

void test_random_insert_erase() {
    const int operations = 1000;
    const int seeds = 100;
    const int integer_range = 25;
    const double erase_prob = 0.4;
    forn(seed, seeds)
    {
        srand(seed);
        treap<int> a;
        std::set<int> s;
        forn(i, operations)
        {
            double rnd = rand() * 1. / RAND_MAX;
            int rnd_el = rand() % integer_range;
            if (rnd < erase_prob)
            { // erase
                a.erase(rnd_el);
                s.erase(rnd_el);
            } 
            else
            { // insert
                a.insert(rnd_el);
                s.insert(rnd_el);
            }
        }
        assert_equals(sz(a), sz(s));
        forit(it, s) assert_true(a.count(*it));
    }
}

template<typename V> double measure(int n) {
    clock_t begin = clock();
    V v;

    forn(i, n) v.insert(i);
    assert_equals(sz(v), n);
    
    forn(i, n) v.insert(i);
    assert_equals(sz(v), n);
    
    forn(i, n) v.erase(i);
    assert_equals(sz(v), 0);
    
    forn(i, n) v.erase(i);
    assert_equals(sz(v), 0);
    
    clock_t end = clock();
    return (end - begin) * 1. / CLOCKS_PER_SEC;
}

void test_performance() {
    const int n = 5000000;
    printf("  set: %.3lf\n", measure< std::set<int> >(n));
    printf("treap: %.3lf\n", measure< treap<int> >(n));
}

void test_lca() {
    treap<int> t;
    const int n = 7;

    int array[] = {1, 3, 8, -5, 6, 1, 10};

    forn(i, n) t.insert(i, array[i]);

    t.hold();

    assert_equals(t.lca(4, 6), 5);
    assert_equals(t.lca(0, 4), 3);
    assert_equals(t.lca(0, 6), 3);
    assert_equals(t.lca(0, 5), 3);
    assert_equals(t.lca(2, 5), 3);
    assert_equals(t.lca(2, 3), 3);
    assert_equals(t.lca(1, 3), 3);
}

int main()
{
    t(test_simple_insert_count_erase);
    t(test_random_insert_erase);
    t(test_lca);
//    t(test_performance);
    return 0;
}

