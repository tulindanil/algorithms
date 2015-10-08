#pragma once

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