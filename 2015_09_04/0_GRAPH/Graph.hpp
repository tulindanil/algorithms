#include "Graph.h"

#pragma mark - Graph

#include <cfloat>
#include <stack>
#include <cmath>
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
    if (Adjacencies.find(vertex) == Adjacencies.end())
        return false;
    
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

#pragma mark - VertexIterator

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

#pragma mark - EdgeIterator

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
//    throw std::runtime_error("bad vertex");
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
//    throw std::runtime_error("bad vertex");
}

#pragma mark - BFS

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
            if (!backEdges.count(eIt->Destination))
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

#pragma mark - Algotithms

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
            if (stack.size() == 0)
                break;
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

template <typename TNode>
std::ostream &operator <<(std::ostream &os, const TGraph<TNode> &g)
{
    for (typename TGraph<TNode>::TAdjacencies::const_iterator pair = g.Adjacencies.begin(); pair != g.Adjacencies.end(); ++pair)
    {
        os << pair->first << ":" << std::endl;
        for (typename TGraph<TNode>::TAdjValue::const_iterator neighbour = pair->second.begin(); neighbour != pair->second.end(); ++neighbour)
        {
            os << neighbour->first << "(" << neighbour->second << ")" << ", ";
        }
        os << std::endl;
    }
    return os;
}
