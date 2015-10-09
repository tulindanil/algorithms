#include <iostream>

template<typename T, typename V>
std::ostream &operator <<(std::ostream &out, const std::pair<T, V> &p)
{
    out << "(" << p.first << ", " << p.second << ")";
    return out;
}


#include <iostream>
#include <set>
#include <map>
#include <stack>
#include <vector>

template<typename TNode>
class graph
{
    
private:
    
    typedef std::map<TNode, float> TAdjValue;
    typedef std::map<TNode, TAdjValue> TAdjacencies;
    TAdjacencies Adjacencies;
    
    void strong(const TNode &vertex,
                std::map<TNode,
                std::pair<std::pair<size_t, size_t>, bool> > &indexes,
                std::stack<TNode> &stack,
                std::set<std::set<TNode> > &stronglyConnected) const;
    
    void DFSVisit(const TNode &vertex,
                  size_t &time,
                  std::map<TNode,
                  std::pair<size_t, size_t> > &schedule) const;
    
public:
    
    typedef std::set<TNode> condensed_type;
    
    inline size_t size() const;
    
    graph();
    graph(const graph &);
    explicit graph(const std::vector<std::pair<TNode, TNode> >& edges);
    
    class const_iterator
    {
        
    private:
        
        typename TAdjacencies::const_iterator InternalIt;
        
    public:
        
        const_iterator(const const_iterator&);
        const_iterator &operator = (const const_iterator&);
        const_iterator(typename TAdjacencies::const_iterator it);
        
        const TNode& operator*() const;
        const TNode* operator->() const;
        bool operator == (const const_iterator&) const;
        bool operator != (const const_iterator&) const;
        const_iterator& operator++();
        
    };
    
    const_iterator begin() const;
    const_iterator end() const;
    
    bool HasVertex(const TNode&) const;
    
    struct edge
    {
        TNode Source;
        TNode Destination;
        float weight;
        
        bool operator <(const edge &rhs) const
        {
            if (Source != rhs.Source)
                return Source < rhs.Source;
            return Destination < rhs.Destination;
        }
    };
    
    class const_edge_iterator
    {
        
    private:
        
        bool isInit;
        typename TAdjValue::const_iterator InternalIt;
        edge InternalEdge;
        
        void Initialize();
        
    public:
        
        const_edge_iterator(const TNode& source, typename TAdjValue::const_iterator it);
        const_edge_iterator(const const_edge_iterator&);
        const_edge_iterator& operator=(const const_edge_iterator&);
        
        const edge &operator* ();
        const edge *operator-> ();
        bool operator == (const const_edge_iterator&) const;
        bool operator != (const const_edge_iterator&) const;
        const_edge_iterator &operator++ ();
        
    };
    
    const_edge_iterator begin(const TNode&) const;
    const_edge_iterator end(const TNode&) const;
    
    bool AddVertex(const TNode&);
    bool AddEdge(const TNode&, const TNode&, const float weight);
    
    bool DeleteVertex(const TNode &);
    
    graph<TNode> transposedGraph() const;
    
    std::map<TNode, TNode> BFS(const TNode &v0) const;
    std::map<TNode, std::pair<size_t, size_t> > DFS() const;
    
    std::pair<std::map<TNode, float>, std::map<TNode, TNode> > dijkstra(const TNode &sourceVertex) const;
    std::pair<std::vector<TNode>, float> shortestPath(const TNode &source,
                                                      const TNode &destination) const;
    
    std::set<std::set<TNode> > SCC() const;
    graph<std::set<TNode> > condensation() const;
    
    template <typename T>
    friend std::ostream &operator <<(std::ostream &os, const graph<T> &g);

};

#include <cfloat>
#include <stack>
#include <math.h>
#include <queue>

#define min(x, y) (((x) > (y)) ? (y) : (x))

template <typename TNode>
graph<TNode>::graph()
{
    
}

template <typename TNode>
graph<TNode>::graph(const graph<TNode>& g): Adjacencies(g.Adjacencies)
{
    
}

template <typename TNode>
inline size_t graph<TNode>::size() const
{
    return Adjacencies.size();
}

template <typename TNode>
bool graph<TNode>::AddEdge(const TNode &src, const TNode &dst, const float weight)
{
    typename std::map<TNode, TAdjValue>::iterator srcIt;
    if ((srcIt = Adjacencies.find(src)) == Adjacencies.end())
        return false;
    
    if (srcIt->second.find(dst) != srcIt->second.end())
        return false;
    
    srcIt->second.insert(std::make_pair(dst, weight));
    
    return true;
}

template <typename TNode>
bool graph<TNode>::AddVertex(const TNode &node)
{
    if(Adjacencies.find(node) != Adjacencies.end())
        return false;
    
    Adjacencies[node] = TAdjValue();
    
    return true;
}

template <typename T>
bool graph<T>::DeleteVertex(const T &vertex)
{
    for (typename TAdjacencies::iterator it = Adjacencies.begin(); it != Adjacencies.end(); ++it)
    {
        it->second.erase(vertex);
    }
    Adjacencies.erase(vertex);
    
    return true;
}

template <typename TNode>
graph<TNode> graph<TNode>::transposedGraph() const
{
    graph<TNode> transposedGraph;
    
    for (typename graph<TNode>::const_iterator vIt = begin(), vItEnd = end(); vIt != vItEnd; ++vIt)
    {
        transposedGraph.AddVertex(*vIt);
    }
    
    for (typename graph<TNode>::const_iterator vIt = begin(), vItEnd = end(); vIt != vItEnd; ++vIt)
    {
        for (typename graph<TNode>::const_edge_iterator edgeIt = begin(*vIt), edgeItEnd = end(*vIt); edgeIt != edgeItEnd; ++edgeIt)
        {
            transposedGraph.AddEdge(edgeIt->Destination, edgeIt->Source, edgeIt->weight);
        }
    }
    
    return transposedGraph;
}


template <typename TNode>
graph<TNode>::const_iterator::const_iterator(typename graph<TNode>::TAdjacencies::const_iterator it): InternalIt(it)
{
    
}

template <typename TNode>
graph<TNode>::const_iterator::const_iterator(const graph<TNode>::const_iterator &rhs): InternalIt(rhs.InternalIt)
{
    
}

template<typename TNode>
typename graph<TNode>::const_iterator &graph<TNode>::const_iterator::operator = (const graph<TNode>::const_iterator& rhs)
{
    InternalIt = rhs.InternalIt;
    return *this;
}

template<typename TNode>
const TNode& graph<TNode>::const_iterator::operator*() const
{
    return InternalIt->first;
}

template<typename TNode>
const TNode* graph<TNode>::const_iterator::operator->() const
{
    return &(InternalIt->first);
}

template<typename TNode>
bool graph<TNode>::const_iterator::operator==(const graph<TNode>::const_iterator& rhs) const
{
    return InternalIt == rhs.InternalIt;
}

template<typename TNode>
bool graph<TNode>::const_iterator::operator!=(const graph<TNode>::const_iterator& rhs) const
{
    return InternalIt != rhs.InternalIt;
}

template<typename TNode>
typename graph<TNode>::const_iterator &graph<TNode>::const_iterator::operator++ ()
{
    ++InternalIt;
    return *this;
}

template<typename TNode>
typename graph<TNode>::const_iterator graph<TNode>::begin() const
{
    return const_iterator(Adjacencies.begin());
}

template<typename TNode>
typename graph<TNode>::const_iterator graph<TNode>::end() const
{
    return const_iterator(Adjacencies.end());
}


template<typename TNode>
void graph<TNode>::const_edge_iterator::Initialize()
{
    if (!isInit)
    {
        isInit = true;
        InternalEdge.Destination = InternalIt->first;
        InternalEdge.weight = InternalIt->second;
    }
}

template<typename TNode>
graph<TNode>::const_edge_iterator::const_edge_iterator(const TNode& source, typename graph<TNode>::TAdjValue::const_iterator it): InternalIt(it), isInit(false)
{
    InternalEdge.Source = source;
}

template<typename TNode>
graph<TNode>::const_edge_iterator::const_edge_iterator(const const_edge_iterator &rhs): InternalIt(rhs.InternalIt), InternalEdge(rhs.InternalEdge)
{
    
}

template<typename TNode>
typename graph<TNode>::const_edge_iterator &graph<TNode>::const_edge_iterator::operator=(const graph<TNode>::const_edge_iterator& rhs)
{
    InternalIt = rhs.InternalIt;
    InternalEdge = rhs.InternalEdge;
    return *this;
}

template<typename TNode>
const typename graph<TNode>::edge& graph<TNode>::const_edge_iterator::operator*()
{
    Initialize();
    return InternalEdge;
}

template<typename TNode>
const typename graph<TNode>::edge* graph<TNode>::const_edge_iterator::operator->()
{
    Initialize();
    return &InternalEdge;
}

template<typename TNode>
bool graph<TNode>::const_edge_iterator::operator== (const graph<TNode>::const_edge_iterator& rhs) const
{
    return InternalIt == rhs.InternalIt;
}

template<typename TNode>
bool graph<TNode>::const_edge_iterator::operator!= (const graph<TNode>::const_edge_iterator& rhs) const
{
    return InternalIt != rhs.InternalIt;
}

template<typename TNode>
typename graph<TNode>::const_edge_iterator &graph<TNode>::const_edge_iterator::operator++ ()
{
    ++InternalIt;
    isInit = false;
    return *this;
}

template<typename TNode>
typename graph<TNode>::const_edge_iterator graph<TNode>::begin(const TNode& v) const
{
    typename TAdjacencies::const_iterator it = Adjacencies.find(v);
    if (it != Adjacencies.end())
    {
        return const_edge_iterator(v, it->second.begin());
    }
    abort();
}

template<typename TNode>
typename graph<TNode>::const_edge_iterator graph<TNode>::end(const TNode& v) const
{
    typename TAdjacencies::const_iterator it = Adjacencies.find(v);
    if (it != Adjacencies.end())
    {
        return const_edge_iterator(v, it->second.end());
    }
    abort();
}


template<typename TNode>
std::map<TNode, TNode> graph<TNode>::BFS(const TNode &v0) const
{
    std::map<TNode, TNode> backEdges;
    std::queue<TNode> queue;
    queue.push(v0);
    backEdges[v0] = v0;
    while (!queue.empty())
    {
        TNode v = queue.front();
        queue.pop();
        for (typename graph<TNode>::const_edge_iterator eIt = begin(v), eEnd = end(v); eIt != eEnd; ++eIt)
        {
            if (backEdges.find(eIt->Destination) == backEdges.end())
            {
                backEdges[eIt->Destination] = eIt->Source;
                queue.push(eIt->Destination);
            }
        }
    }
    return backEdges;
}


template <typename TNode>
void graph<TNode>::DFSVisit(const TNode &vertex, size_t &time, std::map<TNode, std::pair<size_t, size_t> > &schedule) const
{
    schedule[vertex] = std::make_pair(time, 0);
    time++;
    
    for (typename graph<TNode>::const_edge_iterator eIt = begin(vertex), eEnd = end(vertex); eIt != eEnd; ++eIt)
    {
        if (!schedule.count(eIt->Destination) && schedule[eIt->Destination].second == 0)
        {
            DFSVisit(eIt->Destination, time, schedule);
        }
    }
    
    schedule[vertex] = std::make_pair(schedule[vertex].first, time);
    time++;
}

template <typename TNode>
std::map<TNode, std::pair<size_t, size_t> > graph<TNode>::DFS() const
{
    std::map<TNode, std::pair<size_t, size_t> > output;
    size_t time = 1;
    
    for (typename graph<TNode>::const_iterator eIt = begin(), eEnd = end(); eIt != eEnd; ++eIt)
    {
        if (!output.count(*eIt))
        {
            DFSVisit(*eIt, time, output);
        }
    }
    
    return output;
}



size_t f(size_t fst, size_t snd);
template <typename T>
std::pair<std::vector<T>, float> graph<T>::shortestPath(const T &source, const T &destination) const
{
    std::map<T, float> g_score;
    std::map<T, float> f_score;
    
    std::set<T> closed_set, open_set;
    open_set.insert(source);
    
    g_score[source] = 0;
    f_score[source] = 0 + f(source, destination);
    
    
    std::set<std::pair<float, T> > queue;
    queue.insert(std::make_pair(f_score[source], source));
    
    while (open_set.empty() == false)
    {
        T current = queue.begin()->second;
        
        if (current == destination)
            return std::make_pair(std::vector<T>(), g_score[destination]);
        
        open_set.erase(current);
        queue.erase(queue.begin());
        closed_set.insert(current);
        
        for (typename graph<T>::const_edge_iterator edge = this->begin(current); edge != this->end(current); ++edge)
        {
            if (closed_set.find(edge->Destination) != closed_set.end())
                continue;
            
            float alternativeWeight = g_score[current] + edge->weight;
            if (alternativeWeight < g_score[edge->Destination] || open_set.find(edge->Destination) == open_set.end())
            {
                g_score[edge->Destination] = alternativeWeight;
                
                if (open_set.find(edge->Destination) == open_set.end())
                {
                    queue.insert(std::make_pair(f_score[edge->Destination], edge->Destination));
                    open_set.insert(edge->Destination);
                }
                else
                {
                    queue.erase(queue.find(std::make_pair(f_score[edge->Destination], edge->Destination)));
                    queue.insert(std::make_pair(alternativeWeight + f(edge->Destination, destination), edge->Destination));
                }
                
                f_score[edge->Destination] = alternativeWeight + f(edge->Destination, destination);
            }
        }
    }
    return std::make_pair(std::vector<T>(), g_score[destination]);
}


template <typename T>
std::pair<std::map<T, float>, std::map<T, T> > graph<T>::dijkstra(const T &sourceVertex) const
{
    std::map<T, float> destinations;
    std::map<T, T> paths;
    
    destinations[sourceVertex] = 0;
    std::set<std::pair<float, T> > queue;
    
    for (typename graph<T>::const_iterator vertex = this->begin(); vertex != this->end(); ++vertex)
    {
        if (sourceVertex != *vertex)
            destinations[*vertex] = FLT_MAX / 2;
        
        queue.insert(std::make_pair(destinations[*vertex], *vertex));
    }
    
    while (queue.empty() == false)
    {
        std::pair<float, T> pair = *queue.begin();
        queue.erase(queue.begin());
        
        for (typename graph<T>::const_edge_iterator edge = this->begin(pair.second); edge != this->end(pair.second); ++edge)
        {
            float alternativeWeight = pair.first + edge->weight;
            if (alternativeWeight < destinations[edge->Destination])
            {
                std::pair<float, T> dst = *queue.find(std::make_pair(destinations[edge->Destination], edge->Destination));
                queue.erase(dst);
                
                destinations[edge->Destination] = alternativeWeight;
                dst.first = alternativeWeight;
                
                queue.insert(std::make_pair(destinations[edge->Destination], edge->Destination));
                
                paths[edge->Destination] = pair.second;
            }
        }
    }
    return std::make_pair(destinations, paths);
}


template <typename T>
void graph<T>::strong(const T &vertex, std::map<T, std::pair<std::pair<size_t, size_t>, bool> > &indexes, std::stack<T> &stack, std::set<std::set<T> > &stronglyConnected) const
{
    indexes[vertex] = std::make_pair(std::make_pair(indexes.size(), indexes.size()), true);
    stack.push(vertex);
    
    for (typename graph<T>::const_edge_iterator edge = this->begin(vertex); edge != this->end(vertex); ++edge)
    {
        if (indexes.find(edge->Destination) == indexes.end())
        {
            strong(edge->Destination, indexes, stack, stronglyConnected);
            indexes[vertex].first.second = min(indexes[vertex].first.second, indexes[edge->Destination].first.second); // v.lowlink = min(v.lowlink, w.lowlink)
        }
        else if (indexes[edge->Destination].second == true)
        {
            indexes[vertex].first.second = min(indexes[vertex].first.second, indexes[edge->Destination].first.first); // v.lowlink = min(v.lowlink, w.index)
        }
    }
    
    if (indexes[vertex].first.second == indexes[vertex].first.first)
    {
        std::set<T> set;
        T prev;
        do
        {
            prev = stack.top();
            stack.pop();
            indexes[prev].second = false;
            set.insert(prev);
            
        } while (prev != vertex);
        stronglyConnected.insert(set);
    }
}

template <typename T>
std::set<std::set<T> > graph<T>::SCC() const
{
    std::map<T, std::pair<std::pair<size_t, size_t>, bool> > indexes;
    std::set<std::set<T> > set;
    std::stack<T> stack;
    
    for (typename graph<T>::const_iterator it = this->begin(), endIt = this->end(); it != endIt; ++it)
    {
        if (indexes.find(*it) == indexes.end())
            strong(*it, indexes, stack, set);
    }
    
    return set;
}

template <typename T>
graph<std::set<T> > graph<T>::condensation() const
{
    std::set<std::set<T> > scc = SCC();
    
    std::map<T, std::set<T> > vertexComponentMap;
    for (typename std::set<std::set<T> >::iterator component = scc.begin(); component != scc.end(); ++component)
    {
        for (typename std::set<T>::const_iterator vertex = component->begin(); vertex != component->end(); ++vertex)
        {
            vertexComponentMap[*vertex] = *component;
        }
    }
    
    graph<std::set<T> > condensed;
    for (typename std::set<std::set<T> >::const_iterator component = scc.begin(); component != scc.end(); ++component)
    {
        condensed.AddVertex(*component);
        for (typename std::set<T>::const_iterator vertex = component->begin(); vertex != component->end(); ++vertex)
        {
            for (typename graph<T>::const_edge_iterator edgeIt = begin(*vertex); edgeIt != end(*vertex); ++edgeIt)
            {
                if (component->find(edgeIt->Destination) == component->end())
                {
                    condensed.AddVertex(vertexComponentMap[edgeIt->Destination]);
                    condensed.AddEdge(*component, vertexComponentMap[edgeIt->Destination], edgeIt->weight);
                }
            }
        }
    }
    return condensed;
}


template <typename TNode>
std::ostream &operator <<(std::ostream &os, const graph<TNode> &g)
{
    for (typename graph<TNode>::TAdjacencies::const_iterator pair = g.Adjacencies.begin(); pair != g.Adjacencies.end(); ++pair)
    {
        os << pair->first << ":" << std::endl;
        for (typename graph<TNode>::TAdjValue::const_iterator neighbour = pair->second.begin(); neighbour != pair->second.end(); ++neighbour)
        {
            os << neighbour->first << "(" << neighbour->second << ")" << ", ";
        }
        os << std::endl;
    }
    return os;
}
#include <vector>
#include <cmath>
#include <map>
#include <unordered_map>
#include <queue>

#define max(x, y) ((x)>(y) ? (x) : (y))
#define min(x, y) ((x)<(y) ? (x) : (y))

typedef std::pair<int, int> point;
typedef std::pair<point, point> segment;
typedef std::pair<std::string, point> city;

typedef point pt;

inline int area (pt a, pt b, pt c) {
    return (b.first - a.first) * (c.second - a.second) - (b.second - a.second) * (c.first - a.first);
}

inline bool intersect_1 (int a, int b, int c, int d) {
    if (a > b)  std::swap (a, b);
    if (c > d)  std::swap (c, d);
    return max(a,c) <= min(b,d);
}

bool intersect (pt a, pt b, pt c, pt d) {
    return intersect_1 (a.first, b.first, c.first, d.first)
    && intersect_1 (a.second, b.second, c.second, d.second)
    && area(a,b,c) * area(a,b,d) <= 0
    && area(c,d,a) * area(c,d,b) <= 0;
}

std::istream &operator >>(std::istream &in, point &c)
{
    in >> c.first;
    in >> c.second;
    return in;
}

std::istream &operator >>(std::istream &in, segment &s)
{
    in >> s.first;
    in >> s.second;
    return in;
}

std::istream &operator >>(std::istream &in, city &c)
{
    in >> c.first;
    in >> c.second;
    return in;
}

float distance(const city &c1, const city &c2)
{
    return sqrt(pow(c1.second.first - c2.second.first, 2) + pow(c1.second.second - c2.second.second, 2));
}

template <typename T>
void readGraph(std::istream &in, graph<T> &g, std::vector<segment> &ditches)
{
    size_t cities, dithes_qty;
    
    if (!(in >> cities))
    {
        abort();
    }
    
    if (!(in >> dithes_qty))
    {
        abort();
    }
    
    for (size_t i = 0; i < cities; i++)
    {
        T src;
        in >> src;
        g.AddVertex(src);
    }
    
    for (size_t i = 0; i < dithes_qty; i++)
    {
        segment s;
        in >> s;
        ditches.push_back(s);
    }
}

template<typename T>
void out(std::ostream &out, const graph<T> &g)
{
    std::multimap<std::string, std::string> map;
    for (typename graph<T>::const_iterator v1 = g.begin(); v1 != g.end(); ++v1)
    {
        for (typename graph<T>::const_edge_iterator edge = g.begin(*v1); edge != g.end(*v1); ++edge)
        {
            map.insert(std::make_pair(v1->first, edge->Destination.first));
        }
    }
    
    out << map.size() << std::endl;
    for (typename std::multimap<std::string, std::string>::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        out << it->first << " " << it->second << std::endl;
    }
}

const int MAXN = 1000;
int parent[MAXN];

void make_set (int v)
{
    parent[v] = v;
}

int find_set (int v)
{
    if(v == parent[v])
        return v;
    return find_set(parent[v]);
}

void union_sets (int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b)
        parent[b] = a;
}

bool isCycled(int x, int y)
{
    return find_set(x) == find_set(y);
}

template <typename T>
graph<T> minSpanningTree(const graph<T> &g, const T &v0)
{
    graph<T> spanningTree;

    typedef std::pair<float, typename graph<T>::edge> ordered;
    std::set<ordered> queue;
    
    std::map<city, int> numbered;
    
    for (typename graph<T>::const_iterator vertex = g.begin(); vertex != g.end(); ++vertex)
    {
        numbered[*vertex] = numbered.size();
        make_set(numbered[*vertex]);
        for (typename graph<T>::const_edge_iterator edge_it = g.begin(*vertex); edge_it != g.end(*vertex); ++edge_it)
        {
            queue.insert(std::make_pair(edge_it->weight, *edge_it));
        }
    }
    
    for (typename std::set<ordered>::const_iterator ord = queue.begin(); ord != queue.end(); ++ord)
    {
        if (isCycled(numbered[ord->second.Destination], numbered[ord->second.Source]) == false)
        {
            union_sets(numbered[ord->second.Destination], numbered[ord->second.Source]);
            
            spanningTree.AddVertex(ord->second.Source);
            spanningTree.AddVertex(ord->second.Destination);
            spanningTree.AddEdge(ord->second.Source, ord->second.Destination, 1);
        }
    }
    return spanningTree;
}

template <typename T>
void work(graph<T> &g)
{
    std::vector<segment> ditches;
    readGraph(std::cin, g, ditches);
    for (typename graph<T>::const_iterator v1 = g.begin(); v1 != g.end(); ++v1)
    {
        for (typename graph<T>::const_iterator v2 = g.begin(); v2 != g.end(); ++v2)
        {
            if (*v1 == *v2)
                continue;
            
            float path = ::distance(*v1, *v2);
            
            if (ditches.empty() == true)
            {
                g.AddEdge(*v1, *v2, path);
                g.AddEdge(*v2, *v1, path);
            }
            else
            {
                bool isPossible = true;
                for (typename std::vector<segment>::const_iterator s = ditches.begin(); s != ditches.end(); ++s)
                {
                    if (intersect(s->first, s->second, v1->second, v2->second) == true)
                    {
                        isPossible = false;
                    }
                }
                if (isPossible)
                {
                    g.AddEdge(*v1, *v2, path);
                    g.AddEdge(*v2, *v1, path);
                }
            }
        }
    }
    
    if (g.condensation().size() == 1)
    {
        std::cout << "YES" << std::endl;
        graph<T> tree = minSpanningTree(g, *g.begin());
        out(std::cout, tree);
    }
    else
    {
        std::cout << "NO";
    }
}

int main()
{
    graph<city> g;
    work(g);
    return 0;
}