#pragma once

#include <iostream>
#include <set>
#include <map>
#include <stack>
#include <vector>
#include <algorithm>

//typedef std::map<TNode, float> AdjValue<TNode, W>;
//typedef std::map<TNode, AdjValue<TNode, W>> Container;

#define DEPRECATED(func) func __attribute__ ((deprecated))

template<class T>
class vector: public std::vector<std::pair<int, T> > {

public:

    typedef std::vector<std::pair<int, T> > super;

    typename super::iterator find(const T& value) {
        if (this->size() <= value)
            return this->end();
        return this->begin() + value;
    }

    typename super::const_iterator find(const T& value) const {
        if (this->size() <= value)
            return this->end();
        return this->begin() + value;
    }

    T& operator[](const int index) {
        if (this->size() <= index) {
            this->resize(index + 1);
            this->at(index) = std::make_pair(index, T());
        }
        return this->at(index).second;
    }

    void insert(const std::pair<int, T>& p) {
        this->push_back(p);
    }

};

template<class T, class W = float>
struct AdjValue: std::map<T, W> {

};

//template<class W>
//struct AdjValue<int, W>: vector<W> {
//
//};

template<class T, class W = float>
struct Adjancies: std::map<T, AdjValue<T, W> > {

};

template<class W>
struct Adjancies<int, AdjValue<int, W> >: vector<AdjValue<int, W> > {

};

template<typename TNode, typename W = float, typename Container = Adjancies<TNode, W> >
class graph
{

public:
    
    typedef std::set<TNode> condensed_type;
    
    inline size_t size() const;
    
    graph();
    graph(const graph<TNode, W, Container>&);
    explicit graph(const std::vector<std::pair<TNode, TNode> >& edges);
    
    class const_iterator
    {
        
    private:
        
        typename Container::const_iterator InternalIt;
        
    public:
        
        const_iterator(const const_iterator&);
        const_iterator &operator = (const const_iterator&);
        const_iterator(typename Container::const_iterator it);
        
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
        typename AdjValue<TNode, W>::const_iterator InternalIt;
        edge InternalEdge;
        
        void Initialize();
        
    public:
        
        const_edge_iterator(const TNode& source, typename AdjValue<TNode, W>::const_iterator it);
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
    
    bool DEPRECATED(AddVertex(const TNode&));
    bool DEPRECATED(AddEdge(const TNode&, const TNode&, const float weight));
    
    bool DEPRECATED(DeleteVertex(const TNode &));
    bool deleteEdge(const TNode&, const TNode&);

    graph<TNode, W, Container> transposedGraph() const;
    
    std::map<TNode, TNode> BFS(const TNode &v0) const;
    std::map<TNode, std::pair<size_t, size_t> > DFS() const;
    
    std::pair<std::map<TNode, float>, std::map<TNode, TNode> > dijkstra(const TNode &sourceVertex) const;
    std::pair<std::vector<TNode>, float> shortestPath(const TNode &source,
                                                      const TNode &destination) const;
    
    std::set<std::set<TNode> > SCC() const;
    graph<std::set<TNode> > condensation() const;
    
    template <typename T, typename F, typename C>
    friend std::ostream &operator <<(std::ostream &os, const graph<T, F, C> &g);

private:
    
    Container Adjacencies;
    
    void strong(const TNode &vertex,
                std::map<TNode,
                std::pair<std::pair<size_t, size_t>, bool> > &indexes,
                std::stack<TNode> &stack,
                std::set<std::set<TNode> > &stronglyConnected) const;
    
    void DFSVisit(const TNode &vertex,
                  size_t &time,
                  std::map<TNode,
                  std::pair<size_t, size_t> > &schedule) const;
 
};
