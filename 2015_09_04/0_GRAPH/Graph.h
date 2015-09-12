#include <iostream>
#include <set>
#include <map>

#pragma once

template<typename TNode>
class TGraph
{

private:

    typedef std::set<std::pair<TNode, float>> TAdjValue;
    typedef std::map<TNode, TAdjValue> TAdjacencies;
    TAdjacencies Adjacencies;

public:

    inline size_t getVertexCount() const;

    TGraph();
    TGraph(const TGraph &);
    //explicit TGraph(const std::vector<std::pair<TNode, TNode>>& edges);

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

    TGraph<TNode> transposedGraph() const;
};
