#include <queue>
#include <cassert>

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
                    edges.at(row - capacity.begin()).push_back(v - row->begin());
                    edges.at(v - row->begin()).push_back(row - capacity.begin());
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
            maxFlow += flows.at(v - capacity.begin()).at(t); 
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
            flows.at(*v).at(*(v + 1)) += pFlow;
            flows.at(*(v + 1)).at(*v) -= pFlow;
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
            for (typename row::const_iterator n = edges.at(v).begin(); n != edges.at(v).end(); ++n) {
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
        return capacity.at(s).at(t) - flows.at(s).at(t);
    }
    
};
