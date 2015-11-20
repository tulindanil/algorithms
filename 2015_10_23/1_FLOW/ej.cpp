#include <iostream>
#include <string>
#include <map>
#include <set>
#include <cassert>
#include <algorithm>
#include <vector>

std::ostream& operator <<(std::ostream& os, const std::vector<int> &v) {
    for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it)
        os << *it << " ";
    os << std::endl;
    return os;
}

#include <algorithm>

template<class T>
std::ostream &operator <<(std::ostream &os, const std::vector<T>& v) {
    for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
        os << *it << " ";
    }
    return os;
}

template <typename F>
class flow {
public:

    typedef int T;

    typedef std::vector<F> row;
    typedef std::vector<row> matrix;

    static F getMaxFlow(const matrix& cap, const T &s, const T &t) {
        flow<F> instance(cap);
        return instance.maxFlow(s, t);
    }

    flow(const matrix& cap): cap(cap), 
                             flows(matrix(cap.size(), row(cap.size()))),
                             excess(cap.size()),
                             heights(cap.size()),
                             edges(cap.size()) {
         for (typename matrix::const_iterator row = cap.begin(); row != cap.end(); ++row) {
            for (typename row::const_iterator v = row->begin(); v != row->end(); ++v) {
                if (*v > 0) {
                    edges[row - cap.begin()].push_back(v - row->begin());
                    edges[v - row->begin()].push_back(row - cap.begin());
                }
            }
        }
    }

private:

    matrix flows;
    matrix cap;
    matrix edges;

    row excess;
    row heights;

    inline void prePush(const T& s) {
        for (typename row::const_iterator v = edges[s].begin(); v != edges[s].end(); ++v) {
            if (resCap(s, *v) > 0) {
                flows[s][*v] = cap[s][*v];
                flows[*v][s] = -cap[s][*v];
                excess[*v] = cap[s][*v];
                excess[s] -= cap[s][*v];
            }
        }
    }

    F maxFlow(const T &s, const T &t) {
        prePush(s); 
        heights[s] = cap.size();
    
        row esnap = excess; 
        row hsnap = heights;

        while(!excess.empty()) {
            for (typename row::const_iterator overflowed = excess.begin(); overflowed != excess.end(); ++overflowed) {
                T index = overflowed - excess.begin();
                if (*overflowed <= 0 || index == s)
                    continue;

                for (typename row::const_iterator v = edges[index].begin(); v != edges[index].end(); ++v) {
                    F resCapacity = resCap(index, *v);
                    if (resCapacity <= 0)
                        continue;
                    if (heights[index] == heights[*v] + 1)
                        push(index, *v); 
                    if (*overflowed <= 0)
                        break;
                }
           }
            for (typename row::const_iterator overflowed = excess.begin(); overflowed != excess.end(); ++overflowed) {
                T index = overflowed - excess.begin();
                if (index != t && index != s && excess[index] > 0) {
                    relabel(index);
                }
            }

            bool e = (esnap == excess);
            bool h = (hsnap == heights);

            if (e && h)
                break;

            esnap = excess;
            hsnap = heights;
        }
        F value = 0;
        for (typename matrix::const_iterator row = cap.begin(); row != cap.end(); ++row) {
            value += flows[row - cap.begin()][t];
        }
        return value;
    }

    inline bool push(const T &s, const T &t) {
        if (excess[s] <= 0)
            return false;


        F rcap = resCap(s, t);
        F delta = std::min(rcap, excess[s]);

        flows[s][t] += delta;
        flows[t][s] -= delta;

        excess[s] -= delta;
        excess[t] += delta;

        return true;
    }

    bool relabel(const T &vertex) {
        F min_height;
        bool is_init = false;
        for (typename row::const_iterator v = edges[vertex].begin(); v != edges[vertex].end(); ++v) {
            F resCapacity = resCap(vertex, *v);
            if (resCapacity <= 0)
                continue;
            if (!is_init) {
                is_init = true;
                min_height = heights[*v];
            }
            if (heights[vertex] > heights[*v])
                return false;
            min_height = std::min(min_height, heights[*v]);
        }
        if (!is_init)
            return false;
        heights[vertex] = 1 + min_height;
        return true;
    }

    inline F resCap(const T &s, const T &t) const {
        return cap[s][t] - flows[s][t]; 
    }
};

template<class W>
class worker {
    
public:

    static void solve(std::istream &is,
                      std::ostream& os) {
        worker<W> w;
        w.run(is, os); 
    }

private:

    typedef int T;
    typedef std::vector<W> row;
    typedef std::vector<row> matrix;

    matrix capacity;
   
    void run(std::istream &is, 
             std::ostream &os) {
        read(is);
        T src = 0, trg = capacity.size() - 1;
        os << flow<W>::getMaxFlow(capacity, src, trg) << std::endl;
    }

    void read(std::istream &is) {
        size_t s;
        is >> s;
        size_t edges_qty;
        is >> edges_qty;
        capacity = matrix(s, row(s));
        T src, dst; 
        W weight;
        for (size_t index = 0; index < edges_qty; ++index) {
            is >> src >> dst >> weight;
            if (src == dst)
                continue;
            src--;
            dst--;
            capacity[src][dst] += weight;
        }
    }
};

template<class W>
void work() {
    worker<W>::solve(std::cin,
                     std::cout);
}

int main() {
    work<long long int>();
    return 0;
}
