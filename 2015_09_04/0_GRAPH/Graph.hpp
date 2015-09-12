#include "graph.h"

#pragma mark - Graph

template <typename TNode>
TGraph<TNode>::TGraph()
{

}

template <typename TNode>
TGraph<TNode>::TGraph(const TGraph<TNode>& g): Adjacencies(g.Adjacencies)
{

}

template <typename TNode>
inline size_t TGraph<TNode>::getVertexCount() const
{
    return Adjacencies.size();
}

template <typename TNode>
bool TGraph<TNode>::AddEdge(const TNode &src, const TNode &dst, const float weight)
{
    typename std::map<TNode, TAdjValue>::iterator srcIt;
    if ((srcIt = Adjacencies.find(src)) == Adjacencies.end())
        return false;

    if (srcIt->second.find({dst, weight}) != srcIt->second.end())
        return false;

    srcIt->second.insert({dst, weight});

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

template <typename TNode>
TGraph<TNode> TGraph<TNode>::transposedGraph() const
{
    TGraph<TNode> transposedGraph;

    for (auto vIt = GetVerticesBegin(), vItEnd = GetVerticesEnd(); vIt != vItEnd; ++vIt)
    {
        transposedGraph.AddVertex(*vIt);
    }

    for (auto vIt = GetVerticesBegin(), vItEnd = GetVerticesEnd(); vIt != vItEnd; ++vIt)
    {
        for (auto edgeIt = GetVertexNeighboursBegin(*vIt), edgeItEnd = GetVertexNeighboursEnd(*vIt); edgeIt != edgeItEnd; ++edgeIt)
        {
            transposedGraph.AddEdge(edgeIt->Destination, edgeIt->Source);
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
typename TGraph<TNode>::TConstVertexIterator &TGraph<TNode>::TConstVertexIterator::operator= (const TGraph<TNode>::TConstVertexIterator& rhs)
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
    auto it = Adjacencies.find(v);
    if (it != Adjacencies.end())
    {
        return TConstEdgeIterator(v, it->second.begin());
    }
    throw std::runtime_error("bad vertex");
}

template<typename TNode>
typename TGraph<TNode>::TConstEdgeIterator TGraph<TNode>::GetVertexNeighboursEnd(const TNode& v) const
{
    auto it = Adjacencies.find(v);
    if (it != Adjacencies.end())
    {
        return TConstEdgeIterator(v, it->second.end());
    }
    throw std::runtime_error("bad vertex");
}


