#include <iostream>
#include <string>
#include <map>
#include <set>
#include <cassert>
#include <algorithm>
#include <vector>

template <typename T, typename K>
std::ostream &operator <<(std::ostream &os, const std::pair<T, K> &p)
{
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

template <typename T, typename K>
std::ostream &operator <<(std::ostream &os, std::map<T, K> &map)
{
    for (typename std::map<T, K>::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        os << it->first << ", " << it->second << "; "; 
    }
    return os;
}

template <typename T>
std::ostream &operator <<(std::ostream &os, std::vector<T> &v)
{
    for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        os << *it << " "; 
    }
    return os;
}

#include <queue>
#include <cassert>

std::ostream& operator <<(std::ostream& os, const std::vector<int> &v) {
    for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it)
        os << *it << " ";
    os << std::endl;
    return os;
}

template<class F>
class flow {
    
public:
    
    typedef int T;

    typedef std::vector<F> row;
    typedef std::vector<row> matrix;

    static F getMaxFlow(const matrix& capacity,
                        const T& source,
                        const T& target) {
        flow<F> instance(capacity);
        return instance.maxFlow(source, target);
    }
    
private:
    
    matrix flows;
    matrix capacity;
    
    typedef std::vector<T> path;

    matrix edges;
    
    flow(const matrix& capacity)
                : capacity(capacity),
                flows(matrix(capacity.size(), row(capacity.size()))),
                edges(matrix(capacity.size(), row())) { 
        for (typename matrix::const_iterator row = capacity.begin(); row != capacity.end(); ++row) {
            for (typename row::const_iterator v = row->begin(); v != row->end(); ++ v) {
                if (*v > 0) {
                    edges[row - capacity.begin()].push_back(v - row->begin());
                    edges[v - row->begin()].push_back(row - capacity.begin());
                }
            }
        }
    }
    
    F maxFlow(const T& s, const T& t) {
        path p;
        while(!(p = findPath(s, t)).empty()) {
            F pFlow = findPathFlow(p);
            pushFlowThrowPath(p, pFlow);
        }
        return countMaxFlow(s, t);
    }
    
    F countMaxFlow(const T& s, const T& t) const {
        F maxFlow = 0;
        for (typename matrix::const_iterator v = capacity.begin(); v != capacity.end(); ++v) {
            if (v - capacity.begin() == t)
                continue;
            maxFlow += flows[v - capacity.begin()][t]; 
        }
        return maxFlow;
    }
    
    F findPathFlow(const path& p) const {
        F minFlow = resCap(*p.begin(), *(p.begin() + 1));
        for (typename path::const_iterator v = p.begin(); v != p.end() - 1; ++v) {
            minFlow = std::min(minFlow, resCap(*v, *(v + 1)));
        }
        return minFlow;
    }
    
    void pushFlowThrowPath(const path& p, const F& pFlow) {
        for (typename path::const_iterator v = p.begin(); v != p.end() - 1; ++v) {
            flows[*v][*(v + 1)] += pFlow;
            flows[*(v + 1)][*v] -= pFlow;
        }
    }
    
    std::map<T, T> breadth(const T& s) const {
        std::map<T, T> destinyMap;
        std::queue<T> queue;
        queue.push(s);
        destinyMap[s] = s;
        while (!queue.empty()) {
            T v = queue.front();
            queue.pop();
            for (typename row::const_iterator n = edges[v].begin(); n != edges[v].end(); ++n) {
                if (resCap(v, *n) > 0 &&
                    destinyMap.count(*n) == 0) {
                    destinyMap[*n] = v;
                    queue.push(*n);
                }
            }
        }
        return destinyMap;
    }
    
    path findPath(const T& s, const T& t) const {
        std::map<T, T> destinyMap = breadth(s);
        if (destinyMap.find(t) == destinyMap.end())
            return path();
        path p;
        T v = t;
        while(v != s) {
            p.push_back(v);
            v = destinyMap[v];
        }
        p.push_back(v);
        std::reverse(p.begin(), p.end());
        return p;
    }
    
    inline F resCap(const T& s, const T& t) const {
        return capacity[s][t] - flows[s][t];
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
            src--;
            dst--;
            capacity[src][dst] = weight;
        }
    }
};

template<class W>
void work() {
    worker<W>::solve(std::cin,
                     std::cout);
}

int main()
{
    work<long long int>();
    return 0;
}
