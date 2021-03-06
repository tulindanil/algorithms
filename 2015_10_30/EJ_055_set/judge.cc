#include <iostream>
#include <set>
#include <utility>
#include <iostream>
#include <map>

#ifdef DEBUG

template<class K, class T>
std::ostream& operator <<(std::ostream& os, const std::pair<K, T>& p) {
    os << "{" << p.first << ", " << p.second << "}";
    return os;
}

template<class T>
std::ostream& operator <<(std::ostream& os, const std::vector<T>& v) {
    if (v.empty()) return os;
    os << "[";
    for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
        os << *it << (((it - v.begin()) != v.size() - 1) ? ", " : "]"); 
    }
    os << " " << v.size();
    return os;
}

template<class T, class K>
std::ostream& operator <<(std::ostream& os, const std::map<T, K>& map) {
    os << "[";
    for (typename std::map<T, K>::const_iterator it = map.begin(); it != --map.end(); ++it) {
        os << "(" << it->first << ", " << it->second << "), ";
    }
    os << "(" << (--map.end())->first << ", " << (--map.end())->second << ")" << "]";
    return os;
}

#endif
 
#include <iostream>
#include <vector>
#include <cmath>
#include <float.h>
#include <map>
#include <utility>
#include <climits>

#include <vector>

namespace dummy {

class graph {

public:

    typedef int T;
    typedef std::vector<T> adjancies;
    typedef std::vector<adjancies> container;

    graph(const size_t& size): list(size) {

    }
    
    inline void resize(const size_t& size) {
        list.resize(size);
    }

    inline void addEdge(const size_t& src, const size_t& dst) {
        list[src].push_back(dst);
    }

    inline adjancies& operator[](const size_t& index) {
        return list[index];
    }

    typedef adjancies::const_iterator edge_it;
    typedef container::const_iterator it;

    inline it begin() const {
        return list.begin();
    }

    inline it end() const {
        return list.end();
    }

    inline edge_it begin(const size_t& v) const {
        return list[v].begin();
    }

    inline edge_it end(const size_t& v) const {
        return list[v].end();
    }

    inline size_t size() const {
        return list.size();
    }

#ifdef DEBUG

    friend std::ostream& operator <<(std::ostream& os, const graph& g) {
        for (it i = g.list.begin(); i != g.list.end(); ++i) {
            os << i - g.list.begin() << ": " << *i << std::endl;
        }
        return os;
    }
    
#endif

private:

   container list;

};

}

template <typename T, typename K>
std::istream &operator >>(std::istream &in, std::pair<T, K> &p) {
    in >> p.first;
    in >> p.second;
    return in;
}

template<class Queue>
class Solver {

public:

    void static solve(std::istream &is, std::ostream& os) {
        size_t qty = 0;
        is >> qty;
        
        float distance = 0;
        is >> distance;

        Solver<Queue> s = Solver<Queue>(qty, distance);
        s.read(std::cin); 
        s.proceed(std::cout);
    }

#ifdef DEBUG

    template<typename q>
    friend std::ostream& operator <<(std::ostream& os, const Solver<q>& s) {
        os << s.graph << std::endl;
        os << s.cities << std::endl;
        os << s.abroad_cities << std::endl;
        for (typename matrix::const_iterator row = s.m.begin(); row != s.m.end(); ++row) {
            os << *row << std::endl;
        }
        return os;
    }

#endif

private:

    typedef int T;

    typedef float F;
    typedef std::pair<F, F> point;
    typedef std::map<point, T> container;

    typedef std::vector<F> row;
    typedef std::vector<row> matrix;

    matrix m;

    size_t size;
    F distance;

    container cities;
    std::vector<point> abroad_cities;

    dummy::graph graph;

    Solver(size_t s, F d): size(s), distance(d), graph(s), m(s, row(s)) {

    }

    inline void proceedReachibility(const point &p1, const point &p2) {
        F dist = sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
        if (dist < distance) {
            int v1 = cities[p1];
            int v2 = cities[p2];
            m[v1][v2] = dist;
            graph.addEdge(v1, v2);
        }
    }

    inline void read(std::istream& is) {
        point source, destination;
        is >> source >> destination;

        cities[source] = 0;
        cities[destination] = 1;
        point p;
        for (size_t i = 2; i < size; i++) {
            is >> p;
            if (p.first < 0) 
                abroad_cities.push_back(p);
            cities[p] = i;
        }
        for (container::const_iterator v1 = cities.begin(); v1 != cities.end(); ++v1) {
            for (container::const_iterator v2 = cities.begin(); v2 != cities.end(); ++v2) {
                if (*v1 == *v2) 
                    continue;
                proceedReachibility(v1->first, v2->first);
            }
        }
    }

    inline void proceed(std::ostream& os) const {
        F min = FLT_MAX;
        for (std::vector<point>::const_iterator c = abroad_cities.begin(); c != abroad_cities.end(); ++c) {
            int abroad_city = cities.find(*c)->second;
            F fst = shortestPath(0, abroad_city), snd = shortestPath(abroad_city, 1);
            min = std::min(min, fst + snd);
        }
        os << min << std::endl;
    }

    inline F shortestPath(int s, int d) const {
        Queue q;
        std::vector<F> dist = std::vector<F>(size);
        
        typedef dummy::graph::it iterator;
        typedef dummy::graph::edge_it e_iterator;

        for (iterator it = graph.begin(); it != graph.end(); ++it) {
            q.push(std::make_pair(FLT_MAX / 2, (it - graph.begin())));
            dist[it - graph.begin()] = FLT_MAX / 2;
        }

        dist[s] = 0;

        while (!q.empty()) {
            int v = q.top();
            q.pop();
            for (e_iterator e_it = graph.begin(v); e_it != graph.end(v); ++e_it) {
                F alternative = dist[v] + m[v][*e_it];
                if (alternative < dist[*e_it]) {
                    q.replace(std::make_pair(dist[*e_it], *e_it), std::make_pair(alternative, *e_it));
                    dist[*e_it] = alternative;
                }
            }
        }
        return dist[d];
    }

};

template<typename Queue>
void wrap() {
    Solver<Queue>::solve(std::cin, std::cout);     
}

class queued_set: public std::set<std::pair<float, int> > {

public:

   typedef std::pair<float, int> T;

   void push(const T& key) {
       insert(key);
   }

   int top() const {
       return begin()->second;
   }

   void pop() {
        erase(begin());
   }

   void replace(const T& oldKey, const T& newKey) {
       erase(oldKey);
       insert(newKey);
   }

};

int main() {
    wrap<queued_set>();
}
