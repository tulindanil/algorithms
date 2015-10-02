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
class TGraph
{
    
private:
    
    typedef std::map<TNode, float> TAdjValue;
    typedef std::map<TNode, TAdjValue> TAdjacencies;
    TAdjacencies Adjacencies;
    
    void strong(const TNode &vertex,
                std::map<TNode, std::pair<std::pair<size_t, size_t>, bool> > &indexes,
                std::stack<TNode> &stack,
                std::set<std::set<TNode> > &stronglyConnected) const;
    
    void DFSVisit(const TNode &vertex, size_t &time, std::map<TNode, std::pair<size_t, size_t> > &schedule) const;
    
public:
    
    typedef std::set<TNode> condensed_type;
    
    inline size_t size() const;
    
    TGraph();
    TGraph(const TGraph &);
    //explicit TGraph(const std::vector<std::pair<TNode, TNode> >& edges);
    
    class TConstVertexIterator
    {
        
    private:
        
        typename TAdjacencies::const_iterator InternalIt;
        
    public:
        
        TConstVertexIterator(const TConstVertexIterator&);
        TConstVertexIterator &operator = (const TConstVertexIterator&);
        TConstVertexIterator(typename TAdjacencies::const_iterator it);
        
        const TNode& operator*() const;
        const TNode* operator->() const;
        bool operator == (const TConstVertexIterator&) const;
        bool operator != (const TConstVertexIterator&) const;
        TConstVertexIterator& operator++();
        
    };
    
    TConstVertexIterator GetVerticesBegin() const;
    TConstVertexIterator GetVerticesEnd() const;
    
    bool HasVertex(const TNode&) const;
    
    struct TEdge
    {
        TNode Source;
        TNode Destination;
        float weight;
    };
    
    class TConstEdgeIterator
    {
        
    private:
        
        bool isInit;
        typename TAdjValue::const_iterator InternalIt;
        TEdge InternalEdge;
        
        void Initialize();
        
    public:
        
        TConstEdgeIterator(const TNode& source, typename TAdjValue::const_iterator it);
        TConstEdgeIterator(const TConstEdgeIterator&);
        TConstEdgeIterator& operator=(const TConstEdgeIterator&);
        
        const TEdge &operator* ();
        const TEdge *operator-> ();
        bool operator == (const TConstEdgeIterator&) const;
        bool operator != (const TConstEdgeIterator&) const;
        TConstEdgeIterator &operator++ ();
        
    };
    
    TConstEdgeIterator GetVertexNeighboursBegin(const TNode&) const;
    TConstEdgeIterator GetVertexNeighboursEnd(const TNode&) const;
    
    bool AddVertex(const TNode&);
    bool AddEdge(const TNode&, const TNode&, const float weight);
    
    bool DeleteVertex(const TNode &);
    
    TGraph<TNode> transposedGraph() const;
    
    std::map<TNode, TNode> BFS(const TNode &v0) const;
    std::map<TNode, std::pair<size_t, size_t> > DFS() const;
    
    std::pair<std::map<TNode, float>, std::map<TNode, TNode> > dijkstra(const TNode &sourceVertex) const;
    std::pair<std::vector<TNode>, float> shortestPath(const TNode &source, const TNode &destination) const;
    
    std::set<std::set<TNode> > SCC() const;
    std::set<std::set<TNode> > SCC2() const;
    TGraph<std::set<TNode> > condensation() const;
    
    template <typename T>
    friend std::ostream &operator <<(std::ostream &os, const TGraph<T> &g);

};

#include <cfloat>
#include <stack>
#include <math.h>
#include <queue>

#define min(x, y) (((x) > (y)) ? (y) : (x))

template <typename TNode>
TGraph<TNode>::TGraph()
{
    
}

template <typename TNode>
TGraph<TNode>::TGraph(const TGraph<TNode>& g): Adjacencies(g.Adjacencies)
{
    
}

template <typename TNode>
inline size_t TGraph<TNode>::size() const
{
    return Adjacencies.size();
}

template <typename TNode>
bool TGraph<TNode>::AddEdge(const TNode &src, const TNode &dst, const float weight)
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
bool TGraph<TNode>::AddVertex(const TNode &node)
{
    if(Adjacencies.find(node) != Adjacencies.end())
        return false;
    
    Adjacencies[node] = TAdjValue();
    
    return true;
}

template <typename T>
bool TGraph<T>::DeleteVertex(const T &vertex)
{
    for (typename TAdjacencies::iterator it = Adjacencies.begin(); it != Adjacencies.end(); ++it)
    {
        it->second.erase(vertex);
    }
    Adjacencies.erase(vertex);
    
    return true;
}

template <typename TNode>
TGraph<TNode> TGraph<TNode>::transposedGraph() const
{
    TGraph<TNode> transposedGraph;
    
    for (typename TGraph<TNode>::TConstVertexIterator vIt = GetVerticesBegin(), vItEnd = GetVerticesEnd(); vIt != vItEnd; ++vIt)
    {
        transposedGraph.AddVertex(*vIt);
    }
    
    for (typename TGraph<TNode>::TConstVertexIterator vIt = GetVerticesBegin(), vItEnd = GetVerticesEnd(); vIt != vItEnd; ++vIt)
    {
        for (typename TGraph<TNode>::TConstEdgeIterator edgeIt = GetVertexNeighboursBegin(*vIt), edgeItEnd = GetVertexNeighboursEnd(*vIt); edgeIt != edgeItEnd; ++edgeIt)
        {
            transposedGraph.AddEdge(edgeIt->Destination, edgeIt->Source, edgeIt->weight);
        }
    }
    
    return transposedGraph;
}


template <typename TNode>
TGraph<TNode>::TConstVertexIterator::TConstVertexIterator(typename TGraph<TNode>::TAdjacencies::const_iterator it): InternalIt(it)
{
    
}

template <typename TNode>
TGraph<TNode>::TConstVertexIterator::TConstVertexIterator(const TGraph<TNode>::TConstVertexIterator &rhs): InternalIt(rhs.InternalIt)
{
    
}

template<typename TNode>
typename TGraph<TNode>::TConstVertexIterator &TGraph<TNode>::TConstVertexIterator::operator = (const TGraph<TNode>::TConstVertexIterator& rhs)
{
    InternalIt = rhs.InternalIt;
    return *this;
}

template<typename TNode>
const TNode& TGraph<TNode>::TConstVertexIterator::operator*() const
{
    return InternalIt->first;
}

template<typename TNode>
const TNode* TGraph<TNode>::TConstVertexIterator::operator->() const
{
    return &(InternalIt->first);
}

template<typename TNode>
bool TGraph<TNode>::TConstVertexIterator::operator==(const TGraph<TNode>::TConstVertexIterator& rhs) const
{
    return InternalIt == rhs.InternalIt;
}

template<typename TNode>
bool TGraph<TNode>::TConstVertexIterator::operator!=(const TGraph<TNode>::TConstVertexIterator& rhs) const
{
    return InternalIt != rhs.InternalIt;
}

template<typename TNode>
typename TGraph<TNode>::TConstVertexIterator &TGraph<TNode>::TConstVertexIterator::operator++ ()
{
    ++InternalIt;
    return *this;
}

template<typename TNode>
typename TGraph<TNode>::TConstVertexIterator TGraph<TNode>::GetVerticesBegin() const
{
    return TConstVertexIterator(Adjacencies.begin());
}

template<typename TNode>
typename TGraph<TNode>::TConstVertexIterator TGraph<TNode>::GetVerticesEnd() const
{
    return TConstVertexIterator(Adjacencies.end());
}


template<typename TNode>
void TGraph<TNode>::TConstEdgeIterator::Initialize()
{
    if (!isInit)
    {
        isInit = true;
        InternalEdge.Destination = InternalIt->first;
        InternalEdge.weight = InternalIt->second;
    }
}

template<typename TNode>
TGraph<TNode>::TConstEdgeIterator::TConstEdgeIterator(const TNode& source, typename TGraph<TNode>::TAdjValue::const_iterator it): InternalIt(it), isInit(false)
{
    InternalEdge.Source = source;
}

template<typename TNode>
TGraph<TNode>::TConstEdgeIterator::TConstEdgeIterator(const TConstEdgeIterator &rhs): InternalIt(rhs.InternalIt), InternalEdge(rhs.InternalEdge)
{
    
}

template<typename TNode>
typename TGraph<TNode>::TConstEdgeIterator &TGraph<TNode>::TConstEdgeIterator::operator=(const TGraph<TNode>::TConstEdgeIterator& rhs)
{
    InternalIt = rhs.InternalIt;
    InternalEdge = rhs.InternalEdge;
    return *this;
}

template<typename TNode>
const typename TGraph<TNode>::TEdge& TGraph<TNode>::TConstEdgeIterator::operator*()
{
    Initialize();
    return InternalEdge;
}

template<typename TNode>
const typename TGraph<TNode>::TEdge* TGraph<TNode>::TConstEdgeIterator::operator->()
{
    Initialize();
    return &InternalEdge;
}

template<typename TNode>
bool TGraph<TNode>::TConstEdgeIterator::operator== (const TGraph<TNode>::TConstEdgeIterator& rhs) const
{
    return InternalIt == rhs.InternalIt;
}

template<typename TNode>
bool TGraph<TNode>::TConstEdgeIterator::operator!= (const TGraph<TNode>::TConstEdgeIterator& rhs) const
{
    return InternalIt != rhs.InternalIt;
}

template<typename TNode>
typename TGraph<TNode>::TConstEdgeIterator &TGraph<TNode>::TConstEdgeIterator::operator++ ()
{
    ++InternalIt;
    isInit = false;
    return *this;
}

template<typename TNode>
typename TGraph<TNode>::TConstEdgeIterator TGraph<TNode>::GetVertexNeighboursBegin(const TNode& v) const
{
    typename TAdjacencies::const_iterator it = Adjacencies.find(v);
    if (it != Adjacencies.end())
    {
        return TConstEdgeIterator(v, it->second.begin());
    }
    abort();
}

template<typename TNode>
typename TGraph<TNode>::TConstEdgeIterator TGraph<TNode>::GetVertexNeighboursEnd(const TNode& v) const
{
    typename TAdjacencies::const_iterator it = Adjacencies.find(v);
    if (it != Adjacencies.end())
    {
        return TConstEdgeIterator(v, it->second.end());
    }
    abort();
}


template<typename TNode>
std::map<TNode, TNode> TGraph<TNode>::BFS(const TNode &v0) const
{
    std::map<TNode, TNode> backEdges;
    std::queue<TNode> queue;
    queue.push(v0);
    backEdges[v0] = v0;
    while (!queue.empty())
    {
        TNode v = queue.front();
        queue.pop();
        for (typename TGraph<TNode>::TConstEdgeIterator eIt = GetVertexNeighboursBegin(v), eEnd = GetVertexNeighboursEnd(v); eIt != eEnd; ++eIt)
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
void TGraph<TNode>::DFSVisit(const TNode &vertex, size_t &time, std::map<TNode, std::pair<size_t, size_t> > &schedule) const
{
    schedule[vertex] = std::make_pair(time, 0);
    time++;
    
    for (typename TGraph<TNode>::TConstEdgeIterator eIt = GetVertexNeighboursBegin(vertex), eEnd = GetVertexNeighboursEnd(vertex); eIt != eEnd; ++eIt)
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
std::map<TNode, std::pair<size_t, size_t> > TGraph<TNode>::DFS() const
{
    std::map<TNode, std::pair<size_t, size_t> > output;
    size_t time = 1;
    
    for (typename TGraph<TNode>::TConstVertexIterator eIt = GetVerticesBegin(), eEnd = GetVerticesEnd(); eIt != eEnd; ++eIt)
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
std::pair<std::vector<T>, float> TGraph<T>::shortestPath(const T &source, const T &destination) const
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
        
        for (typename TGraph<T>::TConstEdgeIterator edge = this->GetVertexNeighboursBegin(current); edge != this->GetVertexNeighboursEnd(current); ++edge)
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
std::pair<std::map<T, float>, std::map<T, T> > TGraph<T>::dijkstra(const T &sourceVertex) const
{
    std::map<T, float> destinations;
    std::map<T, T> paths;
    
    destinations[sourceVertex] = 0;
    std::set<std::pair<float, T> > queue;
    
    for (typename TGraph<T>::TConstVertexIterator vertex = this->GetVerticesBegin(); vertex != this->GetVerticesEnd(); ++vertex)
    {
        if (sourceVertex != *vertex)
            destinations[*vertex] = FLT_MAX / 2;
        
        queue.insert(std::make_pair(destinations[*vertex], *vertex));
    }
    
    while (queue.empty() == false)
    {
        std::pair<float, T> pair = *queue.begin();
        queue.erase(queue.begin());
        
        for (typename TGraph<T>::TConstEdgeIterator edge = this->GetVertexNeighboursBegin(pair.second); edge != this->GetVertexNeighboursEnd(pair.second); ++edge)
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
void TGraph<T>::strong(const T &vertex, std::map<T, std::pair<std::pair<size_t, size_t>, bool> > &indexes, std::stack<T> &stack, std::set<std::set<T> > &stronglyConnected) const
{
    indexes[vertex] = std::make_pair(std::make_pair(indexes.size(), indexes.size()), true);
    stack.push(vertex);
    
    for (typename TGraph<T>::TConstEdgeIterator edge = this->GetVertexNeighboursBegin(vertex); edge != this->GetVertexNeighboursEnd(vertex); ++edge)
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
std::set<std::set<T> > TGraph<T>::SCC() const
{
    std::map<T, std::pair<std::pair<size_t, size_t>, bool> > indexes;
    std::set<std::set<T> > set;
    std::stack<T> stack;
    
    for (typename TGraph<T>::TConstVertexIterator it = this->GetVerticesBegin(), endIt = this->GetVerticesEnd(); it != endIt; ++it)
    {
        if (indexes.find(*it) == indexes.end())
            strong(*it, indexes, stack, set);
    }
    
    return set;
}

template <typename T>
TGraph<std::set<T> > TGraph<T>::condensation() const
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
    
    TGraph<std::set<T> > condensed;
    for (typename std::set<std::set<T> >::const_iterator component = scc.begin(); component != scc.end(); ++component)
    {
        condensed.AddVertex(*component);
        for (typename std::set<T>::const_iterator vertex = component->begin(); vertex != component->end(); ++vertex)
        {
            for (typename TGraph<T>::TConstEdgeIterator edgeIt = GetVertexNeighboursBegin(*vertex); edgeIt != GetVertexNeighboursEnd(*vertex); ++edgeIt)
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


#include <vector>
#include <cmath>
#include <map>
#include <queue>

#define max(x, y) ((x)>(y) ? (x) : (y))

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
void readGraph(std::istream &in, TGraph<T> &g, std::vector<segment> &ditches)
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

template <typename T>
TGraph<T> minSpanningTree(const TGraph<T> &g, const T &v0)
{
    TGraph<T> spanningTree;
    
    typedef std::pair<float, T> ordered;
    std::set<ordered> queue;
    std::map<T, float> dist;
    std::map<T, T> pathMap;
    
    for (typename TGraph<T>::TConstVertexIterator v = g.GetVerticesBegin(); v != g.GetVerticesEnd(); ++v)
    {
        if (*v != v0)
        {
            dist[*v] = 3.40282347e+38F;
            queue.insert(std::make_pair(3.40282347e+38F, *v));
        }
    }
    
    queue.insert(std::make_pair(0, v0));
    dist[v0] = 0;
    
    while (queue.empty() == false)
    {
        T v = queue.begin()->second;
        queue.erase(queue.begin());
        
        spanningTree.AddVertex(v);
        
        if (pathMap.find(v) != pathMap.end())
        {
            spanningTree.AddVertex(pathMap[v]);
            spanningTree.AddEdge(v, pathMap[v], 1);
        }
        
        for (typename TGraph<T>::TConstEdgeIterator edge = g.GetVertexNeighboursBegin(v); edge != g.GetVertexNeighboursEnd(v); ++edge)
        {
            if (edge->weight < dist[edge->Destination] && queue.find(std::make_pair(dist[edge->Destination], edge->Destination)) != queue.end())
            {
                ordered ord = *queue.find(std::make_pair(dist[edge->Destination], edge->Destination));
                queue.erase(ord);
                ord.first = edge->weight;
                queue.insert(ord);
                
                dist[edge->Destination] = edge->weight;
                pathMap[edge->Destination] = v;
            }
        }
    }
    
    return spanningTree;
}

template<typename T>
std::ostream &operator<<(std::ostream &out, const TGraph<T> &g)
{
    std::map<std::string, std::string> map;
    for (typename TGraph<T>::TConstVertexIterator v1 = g.GetVerticesBegin(); v1 != g.GetVerticesEnd(); ++v1)
    {
        if (g.GetVertexNeighboursBegin(*v1) != g.GetVertexNeighboursEnd(*v1))
            map[v1->first] = g.GetVertexNeighboursBegin(*v1)->Destination.first;
    }
    out << map.size() << std::endl;
    
    for (typename std::map<std::string, std::string>::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        out << it->first << " " << it->second << std::endl;
    }
    
    return out;
}

template <typename T>
void work(TGraph<T> &g)
{
    std::vector<segment> ditches;
    readGraph(std::cin, g, ditches);
    for (typename TGraph<T>::TConstVertexIterator v1 = g.GetVerticesBegin(); v1 != g.GetVerticesEnd(); ++v1)
    {
        for (typename TGraph<T>::TConstVertexIterator v2 = g.GetVerticesBegin(); v2 != g.GetVerticesEnd(); ++v2)
        {
            if (*v1 == *v2)
                continue;
            
            for (typename std::vector<segment>::const_iterator s = ditches.begin(); s != ditches.end(); ++s)
            {
                if (intersect(s->first, s->second, v1->second, v2->second) == false)
                {
                    float path = distance(*v1, *v2);
                    g.AddEdge(*v1, *v2, path);
                    g.AddEdge(*v2, *v1, path);
                }
            }
        }
    }
    if (g.condensation().size() == 1)
    {
        std::cout << "YES" << std::endl;
        TGraph<T> tree = minSpanningTree(g, *g.GetVerticesBegin());
        std::cout << tree;
    }
    else
    {
        std::cout << "NO";
    }
}

int main()
{
    TGraph<city> g;
    work(g);
    return 0;
}