#include <iostream>
#include <string>
#include <map>
#include <set>
#include <cassert>
#include <algorithm>
#include <vector>

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

struct movement
{
    int dx, dy;
    movement(int dx, int dy): dx(dx), dy(dy) { }
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

    std::vector<std::vector<bool> > deck;
    size_t size;
   
    void run(std::istream &is, 
             std::ostream &os) {
        read(is);
        T src = size*size , trg = size*size + 1;
        os << flow<W>::getMaxFlow(capacity, src, trg) << std::endl;
    }

    inline T getIndex(const std::pair<T, T>& pair) {
        return size*pair.first + pair.second;
    }

    void read(std::istream &is) {
        is >> size;
        capacity = matrix(size*size + 2, row(size*size + 2));
        T src = size*size, dst = size*size + 1; 

        movement moves[] = {movement(-1, 0), movement(1, 0), movement(0, 1), movement(0, -1)};
        deck = std::vector<std::vector<bool> >(size, std::vector<bool>(size));

        for (size_t row = 0; row < size; ++row) {
            std::string s;
            is >> s;
            for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
                deck[row][it - s.begin()] = (*it == '#') ? true : false;
            }
        }

        for (size_t row = 0; row < size; ++row) {
            for (size_t column = 0; column < size; ++column) {
                if (deck[row][column] == true)
                    continue;

                std::pair<T, T> cell = std::make_pair(row, column);

                bool isWhite = (row + column) % 2;
                bool isBlack = !isWhite;

                if (isWhite == true) {
                    capacity[src][getIndex(cell)] = 1;
                    for (size_t i = 0; i < 4; ++i)
                        shift(moves[i], cell);
                }
                else 
                    capacity[getIndex(cell)][dst] = 1;
            }
        }

    }

    inline void shift(const movement &move,
               const std::pair<T, T> &currentCell) {
        T index = getIndex(currentCell) + 3*move.dx + move.dy;
        
        T row = currentCell.first + move.dx;
        T column = currentCell.second + move.dy;

        if ((row < 0 || row == size) || (column < 0 || column == size) || deck.at(row).at(column) == true)
            return;
        capacity[getIndex(currentCell)][index] = 1;
    }
};

template<class W>
void work() {
    worker<W>::solve(std::cin,
                     std::cout);
}

int main() {
    work<int>();
    return 0;
}
