#include "Graph.h"

#pragma mark - Graph

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

#pragma mark - VertexIterator

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

#pragma mark - EdgeIterator

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
//    throw std::runtime_error("bad vertex");
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
//    throw std::runtime_error("bad vertex");
}

#pragma mark - BFS

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

#pragma mark - DFS

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

#pragma mark - Algotithms

#pragma mark - Shortest Path

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
    
//    for (typename graph<T>::const_iterator vertex = this->begin(); vertex != this->end(); ++vertex)
//    {
//        if (source != *vertex)
//        {
//            g_score[*vertex] = FLT_MAX;
//            f_score[*vertex] = FLT_MAX;
//        }
//    }
    
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

#pragma mark - Dijkstra

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

#pragma mark - SCC

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

#pragma mark - operator <<

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
