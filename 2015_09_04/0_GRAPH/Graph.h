#pragma once

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