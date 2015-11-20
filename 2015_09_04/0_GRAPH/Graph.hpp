#include "Graph.h"

#pragma mark - Graph

#include <cfloat>
#include <stack>
#include <math.h>
#include <queue>

#define min(x, y) (((x) > (y)) ? (y) : (x))

template<typename T, typename W, typename Container>
graph<T, W, Container>::graph()
{
    
}

template<typename T, typename W, typename Container>
graph<T, W, Container>::graph(const graph<T, W, Container>& g): Adjacencies(g.Adjacencies)
{
    
}

template<typename T, typename W, typename Container>
inline size_t graph<T, W, Container>::size() const
{
    return Adjacencies.size();
}

template<typename T, typename W, typename Container>
bool graph<T, W, Container>::AddEdge(const T &src, const T &dst, const float weight)
{
    typename Container::iterator srcIt;
    if ((srcIt = Adjacencies.find(src)) == Adjacencies.end())
        return false;
    
    if (srcIt->second.find(dst) != srcIt->second.end())
        return false;
    
    srcIt->second.insert(std::make_pair(dst, weight));
    return true;
}

template<typename T, typename W, typename Container>
bool graph<T, W, Container>::AddVertex(const T &node)
{
    if(Adjacencies.find(node) != Adjacencies.end())
        return false;
    
    Adjacencies[node] = AdjValue<T, W>();
    return true;
}

template<typename T, typename W, typename Container>
bool graph<T, W, Container>::DeleteVertex(const T &vertex)
{
    for (typename Container::iterator it = Adjacencies.begin(); it != Adjacencies.end(); ++it)
    {
        it->second.erase(vertex);
    }
    Adjacencies.erase(vertex);
    
    return true;
}

template<typename T, typename W, typename Container>
bool graph<T, W, Container>::deleteEdge(const T &src, const T &dst)
{
    if (Adjacencies.find(src) == Adjacencies.end()
        || Adjacencies.find(src)->second.find(dst) == Adjacencies.find(src)->second.end())
        return false;

    Adjacencies.find(src)->second.erase(dst);
    return true;
}

template<typename T, typename W, typename Container>
graph<T, W, Container> graph<T, W, Container>::transposedGraph() const
{
    graph<T, W, Container> transposedGraph;
    
    for (typename graph<T, W, Container>::const_iterator vIt = begin(), vItEnd = end(); vIt != vItEnd; ++vIt)
    {
        transposedGraph.AddVertex(*vIt);
    }
    
    for (typename graph<T, W, Container>::const_iterator vIt = begin(), vItEnd = end(); vIt != vItEnd; ++vIt)
    {
        for (typename graph<T, W, Container>::const_edge_iterator edgeIt = begin(*vIt), edgeItEnd = end(*vIt); edgeIt != edgeItEnd; ++edgeIt)
        {
            transposedGraph.AddEdge(edgeIt->Destination, edgeIt->Source, edgeIt->weight);
        }
    }
    
    return transposedGraph;
}

template<typename T, typename W, typename Container>
bool graph<T, W, Container>::hasEdge(const T &src, const T &dst) const
{
    if (Adjacencies.find(src) == Adjacencies.end()
         || Adjacencies.find(src)->second.find(dst) == Adjacencies.find(src)->second.end())
         return false;

    return true;
}

#pragma mark - VertexIterator

template<typename T, typename W, typename Container>
graph<T, W, Container>::const_iterator::const_iterator(typename Container::const_iterator it): InternalIt(it)
{
    
}

template<typename T, typename W, typename Container>
graph<T, W, Container>::const_iterator::const_iterator(const graph<T, W, Container>::const_iterator &rhs): InternalIt(rhs.InternalIt)
{
    
}

template<typename T, typename W, typename Container>
typename graph<T, W, Container>::const_iterator &graph<T, W, Container>::const_iterator::operator = (const graph<T, W, Container>::const_iterator& rhs)
{
    InternalIt = rhs.InternalIt;
    return *this;
}

template<typename T, typename W, typename Container>
const T& graph<T, W, Container>::const_iterator::operator*() const
{
    return InternalIt->first;
}

template<typename T, typename W, typename Container>
const T* graph<T, W, Container>::const_iterator::operator->() const
{
    return &(InternalIt->first);
}

template<typename T, typename W, typename Container>
bool graph<T, W, Container>::const_iterator::operator==(const graph<T, W, Container>::const_iterator& rhs) const
{
    return InternalIt == rhs.InternalIt;
}

template<typename T, typename W, typename Container>
bool graph<T, W, Container>::const_iterator::operator!=(const graph<T, W, Container>::const_iterator& rhs) const
{
    return InternalIt != rhs.InternalIt;
}

template<typename T, typename W, typename Container>
typename graph<T, W, Container>::const_iterator &graph<T, W, Container>::const_iterator::operator++ ()
{
    ++InternalIt;
    return *this;
}

template<typename T, typename W, typename Container>
typename graph<T, W, Container>::const_iterator graph<T, W, Container>::begin() const
{
    return const_iterator(Adjacencies.begin());
}

template<typename T, typename W, typename Container>
typename graph<T, W, Container>::const_iterator graph<T, W, Container>::end() const
{
    return const_iterator(Adjacencies.end());
}

#pragma mark - EdgeIterator

template<typename T, typename W, typename Container>
void graph<T, W, Container>::const_edge_iterator::Initialize()
{
    if (!isInit)
    {
        isInit = true;
        InternalEdge.Destination = InternalIt->first;
        InternalEdge.weight = InternalIt->second;
    }
}

template<typename T, typename W, typename Container>
graph<T, W, Container>::const_edge_iterator::const_edge_iterator(const T& source, typename AdjValue<T, W>::const_iterator it): InternalIt(it), isInit(false)
{
    InternalEdge.Source = source;
}

template<typename T, typename W, typename Container>
graph<T, W, Container>::const_edge_iterator::const_edge_iterator(const const_edge_iterator &rhs): InternalIt(rhs.InternalIt), InternalEdge(rhs.InternalEdge)
{
    
}

template<typename T, typename W, typename Container>
typename graph<T, W, Container>::const_edge_iterator &graph<T, W, Container>::const_edge_iterator::operator=(const graph<T, W, Container>::const_edge_iterator& rhs)
{
    InternalIt = rhs.InternalIt;
    InternalEdge = rhs.InternalEdge;
    return *this;
}

template<typename T, typename W, typename Container>
const typename graph<T, W, Container>::edge& graph<T, W, Container>::const_edge_iterator::operator*()
{
    Initialize();
    return InternalEdge;
}

template<typename T, typename W, typename Container>
const typename graph<T, W, Container>::edge* graph<T, W, Container>::const_edge_iterator::operator->()
{
    Initialize();
    return &InternalEdge;
}

template<typename T, typename W, typename Container>
bool graph<T, W, Container>::const_edge_iterator::operator== (const graph<T, W, Container>::const_edge_iterator& rhs) const
{
    return InternalIt == rhs.InternalIt;
}

template<typename T, typename W, typename Container>
bool graph<T, W, Container>::const_edge_iterator::operator!= (const graph<T, W, Container>::const_edge_iterator& rhs) const
{
    return InternalIt != rhs.InternalIt;
}

template<typename T, typename W, typename Container>
typename graph<T, W, Container>::const_edge_iterator &graph<T, W, Container>::const_edge_iterator::operator++ ()
{
    ++InternalIt;
    isInit = false;
    return *this;
}

template<typename T, typename W, typename Container>
typename graph<T, W, Container>::const_edge_iterator graph<T, W, Container>::begin(const T& v) const
{
    typename Container::const_iterator it = Adjacencies.find(v);
    if (it != Adjacencies.end())
    {
        return const_edge_iterator(v, it->second.begin());
    }
    abort();
//    throw std::runtime_error("bad vertex");
}

template<typename T, typename W, typename Container>
typename graph<T, W, Container>::const_edge_iterator graph<T, W, Container>::end(const T& v) const
{
    typename Container::const_iterator it = Adjacencies.find(v);
    if (it != Adjacencies.end())
    {
        return const_edge_iterator(v, it->second.end());
    }
    abort();
//    throw std::runtime_error("bad vertex");
}

#pragma mark - BFS

template<typename T, typename W, typename Container>
std::map<T, T> graph<T, W, Container>::BFS(const T &v0) const
{
    std::map<T, T> backEdges;
    std::queue<T> queue;
    queue.push(v0);
    backEdges[v0] = v0;
    while (!queue.empty())
    {
        T v = queue.front();
        queue.pop();
        for (typename graph<T, W, Container>::const_edge_iterator eIt = begin(v), eEnd = end(v); eIt != eEnd; ++eIt)
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

template<typename T, typename W, typename Container>
void graph<T, W, Container>::DFSVisit(const T &vertex, size_t &time, std::map<T, std::pair<size_t, size_t> > &schedule) const
{
    schedule[vertex] = std::make_pair(time, 0);
    time++;
    
    for (typename graph<T, W, Container>::const_edge_iterator eIt = begin(vertex), eEnd = end(vertex); eIt != eEnd; ++eIt)
    {
        if (!schedule.count(eIt->Destination) && schedule[eIt->Destination].second == 0)
        {
            DFSVisit(eIt->Destination, time, schedule);
        }
    }
    
    schedule[vertex] = std::make_pair(schedule[vertex].first, time);
    time++;
}

template<typename T, typename W, typename Container>
std::map<T, std::pair<size_t, size_t> > graph<T, W, Container>::DFS() const
{
    std::map<T, std::pair<size_t, size_t> > output;
    size_t time = 1;
    
    for (typename graph<T, W, Container>::const_iterator eIt = begin(), eEnd = end(); eIt != eEnd; ++eIt)
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
template<typename T, typename W, typename Container>
std::pair<std::vector<T>, float> graph<T, W, Container>::shortestPath(const T &source, const T &destination) const
{
    std::map<T, float> g_score;
    std::map<T, float> f_score;
    
    std::set<T> closed_set, open_set;
    open_set.insert(source);
    
    g_score[source] = 0;
    f_score[source] = 0 + f(source, destination);
    
//    for (typename graph<T, W, Container>::const_iterator vertex = this->begin(); vertex != this->end(); ++vertex)
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
        
        for (typename graph<T, W, Container>::const_edge_iterator edge = this->begin(current); edge != this->end(current); ++edge)
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

template<typename T, typename W, typename Container>
std::pair<std::map<T, float>, std::map<T, T> > graph<T, W, Container>::dijkstra(const T &sourceVertex) const
{
    std::map<T, float> destinations;
    std::map<T, T> paths;
    
    destinations[sourceVertex] = 0;
    std::set<std::pair<float, T> > queue;
    
    for (typename graph<T, W, Container>::const_iterator vertex = this->begin(); vertex != this->end(); ++vertex)
    {
        if (sourceVertex != *vertex)
            destinations[*vertex] = FLT_MAX / 2;
        
        queue.insert(std::make_pair(destinations[*vertex], *vertex));
    }
    
    while (queue.empty() == false)
    {
        std::pair<float, T> pair = *queue.begin();
        queue.erase(queue.begin());
        
        for (typename graph<T, W, Container>::const_edge_iterator edge = this->begin(pair.second); edge != this->end(pair.second); ++edge)
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

template<typename T, typename W, typename Container>
void graph<T, W, Container>::strong(const T &vertex, std::map<T, std::pair<std::pair<size_t, size_t>, bool> > &indexes, std::stack<T> &stack, std::set<std::set<T> > &stronglyConnected) const
{
    indexes[vertex] = std::make_pair(std::make_pair(indexes.size(), indexes.size()), true);
    stack.push(vertex);
    
    for (typename graph<T, W, Container>::const_edge_iterator edge = this->begin(vertex); edge != this->end(vertex); ++edge)
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

template<typename T, typename W, typename Container>
std::set<std::set<T> > graph<T, W, Container>::SCC() const
{
    std::map<T, std::pair<std::pair<size_t, size_t>, bool> > indexes;
    std::set<std::set<T> > set;
    std::stack<T> stack;
    
    for (typename graph<T, W, Container>::const_iterator it = this->GetVerticesBegin(), endIt = this->GetVerticesEnd(); it != endIt; ++it)
    {
        if (indexes.find(*it) == indexes.end())
            strong(*it, indexes, stack, set);
    }
    
    return set;
}

template<typename T, typename W, typename Container>
graph<std::set<T> > graph<T, W, Container>::condensation() const
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
            for (typename graph<T, W, Container>::const_edge_iterator edgeIt = GetVertexNeighboursBegin(*vertex); edgeIt != GetVertexNeighboursEnd(*vertex); ++edgeIt)
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

#pragma mark - operator <<

template<typename T, typename W, typename Container>
std::ostream &operator <<(std::ostream &os, const graph<T, W, Container> &g)
{
    for (typename Container::const_iterator pair = g.Adjacencies.begin(); pair != g.Adjacencies.end(); ++pair)
    {
        os << pair->first << ":" << std::endl;
        for (typename AdjValue<T, W>::const_iterator neighbour = pair->second.begin(); neighbour != pair->second.end(); ++neighbour)
        {
            os << neighbour->first << "(" << neighbour->second << ")" << ", ";
        }
        os << std::endl;
    }
    return os;
}
