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
    bool hasEdge(const TNode &src, const TNode &dst) const;
    

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
    bool deleteEdge(const TNode&, const TNode&);

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

template <typename T>
bool graph<T>::deleteEdge(const T &src, const T &dst)
{
    if (Adjacencies.find(src) == Adjacencies.end()
        || Adjacencies.find(src)->second.find(dst) == Adjacencies.find(src)->second.end())
        return false;

    Adjacencies.find(src)->second.erase(dst);
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

template <typename T>
bool graph<T>::hasEdge(const T &src, const T &dst) const
{
    if (Adjacencies.find(src) == Adjacencies.end()
         || Adjacencies.find(src)->second.find(dst) == Adjacencies.find(src)->second.end())
         return false;

    return true;
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
        
        for (typename graph<T>::const_edge_iterator TNode = this->begin(current); TNode != this->end(current); ++TNode)
        {
            if (closed_set.find(TNode->Destination) != closed_set.end())
                continue;
            
            float alternativeWeight = g_score[current] + TNode->weight;
            if (alternativeWeight < g_score[TNode->Destination] || open_set.find(TNode->Destination) == open_set.end())
            {
                g_score[TNode->Destination] = alternativeWeight;
                
                if (open_set.find(TNode->Destination) == open_set.end())
                {
                    queue.insert(std::make_pair(f_score[TNode->Destination], TNode->Destination));
                    open_set.insert(TNode->Destination);
                }
                else
                {
                    queue.erase(queue.find(std::make_pair(f_score[TNode->Destination], TNode->Destination)));
                    queue.insert(std::make_pair(alternativeWeight + f(TNode->Destination, destination), TNode->Destination));
                }
                
                f_score[TNode->Destination] = alternativeWeight + f(TNode->Destination, destination);
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
        
        for (typename graph<T>::const_edge_iterator TNode = this->begin(pair.second); TNode != this->end(pair.second); ++TNode)
        {
            float alternativeWeight = pair.first + TNode->weight;
            if (alternativeWeight < destinations[TNode->Destination])
            {
                std::pair<float, T> dst = *queue.find(std::make_pair(destinations[TNode->Destination], TNode->Destination));
                queue.erase(dst);
                
                destinations[TNode->Destination] = alternativeWeight;
                dst.first = alternativeWeight;
                
                queue.insert(std::make_pair(destinations[TNode->Destination], TNode->Destination));
                
                paths[TNode->Destination] = pair.second;
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
    
    for (typename graph<T>::const_edge_iterator TNode = this->begin(vertex); TNode != this->end(vertex); ++TNode)
    {
        if (indexes.find(TNode->Destination) == indexes.end())
        {
            strong(TNode->Destination, indexes, stack, stronglyConnected);
            indexes[vertex].first.second = min(indexes[vertex].first.second, indexes[TNode->Destination].first.second); // v.lowlink = min(v.lowlink, w.lowlink)
        }
        else if (indexes[TNode->Destination].second == true)
        {
            indexes[vertex].first.second = min(indexes[vertex].first.second, indexes[TNode->Destination].first.first); // v.lowlink = min(v.lowlink, w.index)
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
    
    for (typename graph<T>::const_iterator it = this->GetVerticesBegin(), endIt = this->GetVerticesEnd(); it != endIt; ++it)
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
    
    std::map<T, size_t> vertexComponentMap;
    size_t index = 0;
    for (typename std::set<std::set<T> >::iterator component = scc.begin(); component != scc.end(); ++component)
    {
        for (typename std::set<T>::const_iterator vertex = component->begin(); vertex != component->end(); ++vertex)
        {
            vertexComponentMap[*vertex] = index;
        }
        index++;
    }
    
    graph<std::set<T> > condensed;
    for (typename std::set<std::set<T> >::const_iterator component = scc.begin(); component != scc.end(); ++component)
    {
        condensed.AddVertex(*component);
        for (typename std::set<T>::const_iterator vertex = component->begin(); vertex != component->end(); ++vertex)
        {
            for (typename graph<T>::const_edge_iterator edgeIt = GetVertexNeighboursBegin(*vertex); edgeIt != GetVertexNeighboursEnd(*vertex); ++edgeIt)
            {
                if (component->find(edgeIt->Destination) == component->end())
                {
                    typename std::set<std::set<T> >::iterator requiredSCC = scc.begin();
                    std::advance(requiredSCC, vertexComponentMap[edgeIt->Destination]);
                    condensed.AddVertex(*requiredSCC);
                    condensed.AddEdge(*component, *requiredSCC, edgeIt->weight);
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
template <typename T>
bool isReachable(const graph<T> &g, const T &s, const T &t, const std::map<T, T> destinyMap)
{
    if (destinyMap.find(t) != destinyMap.end())
        return true;
    return false;
}

template <typename T>
std::vector<T> findPath(const graph<T> &g, const T &s, const T &t)
{
    std::map<T, T> destinyMap = g.BFS(s);
    if (isReachable(g, s, t, destinyMap) == false)
        return std::vector<T>();

    std::vector<T> path;

    T current_vertex = t;
    while (current_vertex != s)
    {
        path.push_back(current_vertex);
        current_vertex = destinyMap[current_vertex];
    }
    path.push_back(s);
    std::reverse(path.begin(), path.end());

    return path;
}

template <typename T>
float resudialCapacity(const graph<T> &g,
                       const std::map<std::pair<T, T>, float> &flows,
                       const std::map<std::pair<T, T>, float> &capacity,
                       const T &src,
                       const T &dst)
{
    std::pair<T, T> pair = std::make_pair(src, dst);
    if (g.hasEdge(src, dst) == true)
        return capacity.find(pair)->second - flows.find(pair)->second;
    else if(g.hasEdge(dst, src) == true)
        return -flows.find(pair)->second;

    return 0;
}

template <typename T>
float maxFlow(const graph<T> &g, const T &source, const T &target)
{
    graph<T> resudial_net = g;
    std::map<std::pair<T, T>, float> flows, capacity;
    for (typename graph<T>::const_iterator vertex = g.begin(); vertex != g.end(); ++vertex)
    {
        for (typename graph<T>::const_edge_iterator edge = g.begin(*vertex); edge != g.end(*vertex); ++edge)
        {
            std::pair<T, T> pair = std::make_pair(edge->Source, edge->Destination);
            std::pair<T, T> r_pair = std::make_pair(pair.second, pair.first);  
            flows[pair] = 0;
            flows[r_pair] = 0;

            capacity[pair] = edge->weight;
        }
    }

    std::vector<T> path;
    while((path = findPath(resudial_net, source, target)).size() > 0)
    {
        float min_flow = FLT_MAX;

        for (typename std::vector<T>::const_iterator vertex = path.begin(); vertex != path.end() - 1; ++vertex)
        {
            std::pair<T, T> pair = std::make_pair(*vertex, *(vertex + 1));
            float c_flow = resudialCapacity(g, flows, capacity, pair.first, pair.second); 
            min_flow = min(min_flow, c_flow);
        }
        
        for (typename std::vector<T>::const_iterator vertex = path.begin(); vertex != path.end() - 1; ++vertex)
        {
            std::pair<T, T> pair = std::make_pair(*vertex, *(vertex + 1));
            std::pair<T, T> rev_pair = std::make_pair(*(vertex + 1), *vertex);

            flows[pair] += min_flow;
            flows[rev_pair] = -flows[pair];

            if (resudialCapacity(g,
                                 flows, 
                                 capacity, 
                                 pair.first, 
                                 pair.second) > 0)
                resudial_net.AddEdge(pair.first, pair.second, 1);
            else
                resudial_net.deleteEdge(pair.first, pair.second);
       }
    }

    float flow = 0;

    for (typename graph<T>::const_iterator vertex = g.begin(); vertex != g.end(); ++vertex)
    {
        if (*vertex == source)
            continue;

        flow += flows[std::make_pair(source, *vertex)]; 
    }
    return flow;
}

void workOutUpCell(graph<std::pair<int, int> > &g,
                   const std::vector<std::vector<bool> > &deck, 
                   const size_t size, 
                   const std::pair<int, int> &currentCell)
{
    size_t row = currentCell.first, column = currentCell.second;
    if (row == 0 || deck.at(row - 1).at(column) == 1)
        return;

    std::pair<int, int> upCell = std::make_pair(row - 1, column);
    g.AddEdge(currentCell, upCell, 1);
}

void workOutDownCell(graph<std::pair<int, int> > &g,
                     const std::vector<std::vector<bool> > &deck, 
                     const size_t size, 
                     const std::pair<int, int> &currentCell)
{
    size_t row = currentCell.first, column = currentCell.second;
    if (row == size - 1 || deck.at(row + 1).at(column) == 1)
        return;

    std::pair<int, int> downCell = std::make_pair(row + 1, column);
    g.AddVertex(downCell);
    g.AddEdge(currentCell, downCell, 1);
}

void workOutLeftCell(graph<std::pair<int, int> > &g,
                     const std::vector<std::vector<bool> > &deck, 
                     const size_t size, 
                     const std::pair<int, int> &currentCell)
{
    size_t row = currentCell.first, column = currentCell.second;
    if (column == 0 || deck.at(row).at(column - 1) == 1)
        return;

    std::pair<int, int> leftCell = std::make_pair(row, column - 1);
    g.AddEdge(currentCell, leftCell, 1);
}

void workOutRightCell(graph<std::pair<int, int> > &g,
                      const std::vector<std::vector<bool> > &deck, 
                      const size_t size, 
                      const std::pair<int, int> &currentCell)
{
    size_t row = currentCell.first, column = currentCell.second;
    if (column == size - 1 || deck.at(row).at(column + 1) == 1)
        return;

    std::pair<int, int> rightCell = std::make_pair(row, column + 1);
    g.AddVertex(rightCell);
    g.AddEdge(currentCell, rightCell, 1);
}

void readGraph(std::istream &is,
               graph<std::pair<int, int> > &g, 
               const std::pair<int, int> &source, 
               const std::pair<int, int> &target)
{
    size_t size = 0;
    is >> size;

    std::vector<std::vector<bool> > deck(size, std::vector<bool>(size, 0));

    for (size_t row = 0; row < size; ++row)
    {
        std::string s;
        is >> s;
        for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
        {
            deck[row][it - s.begin()] = (*it == '#') ? true : false;
        }
    }

    g.AddVertex(source);
    g.AddVertex(target);

    for (size_t row = 0; row < size; ++row)
    {
        for (size_t column = 0; column < size; ++column)
        {
            if (deck[row][column] == 1)
                continue;

            std::pair<int, int> cell = std::make_pair(row, column);

            g.AddVertex(cell);

            bool isWhite = ((row % 2 == 0) && (column % 2 == 0)) ||
                           ((row % 2 == 1) && (column % 2 == 1));

            bool isBlack = !isWhite;


            if (isWhite == true)
            {
                g.AddEdge(source, cell, 1);

                workOutUpCell(g, deck, size, cell);
                workOutLeftCell(g, deck, size, cell);
                workOutRightCell(g, deck, size, cell);
                workOutDownCell(g, deck, size, cell);
            }

            if (isBlack == true)
                g.AddEdge(cell, target, 1);
        }
    }
}

int main()
{
    graph<std::pair<int, int> > g;
    std::pair<int, int> source = std::make_pair(-1, 0),
                        target = std::make_pair(0, -1);

    readGraph(std::cin, g, source, target);
    std::cout << maxFlow(g, source, target) << std::endl;

    return 0;
}
