#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>

template <typename T, typename K>
std::ostream &operator <<(std::ostream &os, const std::pair<T, K> &pair)
{
    os << "(" << pair.first << ", " << pair.second << ")";
    return os;
}

template <typename T>
std::ostream &operator <<(std::ostream &os, const std::set<T> &set)
{
    for (auto it: set)
    {
        os << it << ", ";
    }
    return os;
}

template <typename T, typename K>
std::ostream &operator <<(std::ostream &os, const std::map<T, K> &map)
{
    for (auto it: map)
    {
        os << it.first << " " << it.second << "; ";
    }
    return os;
}

template <typename T>
std::ostream &operator <<(std::ostream &os, const std::vector<T> &set)
{
    for (auto it: set)
    {
        os << it << " ";
    }
    return os;
}

#include "Graph.hpp"

template<typename T>
std::istream &operator >>(std::istream &in, TGraph<T> &g)
{
    size_t vertices, edges;
    
    if (!(in >> vertices))
        throw std::runtime_error("bad graph format: vertices");
    
    if (!(in >> edges))
        throw std::runtime_error("bad graph format: edges");
    
    for (size_t i = 0; i < edges; i++)
    {
        T src;
        if (!(in >> src))
            throw std::runtime_error("bad graph format: src");
        
        g.AddVertex(src);
        
        T dst;
        if (!(in >> dst))
            throw std::runtime_error("bad graph format: dst");
        
        g.AddVertex(dst);
        g.AddEdge(src, dst, 1);
    }
    
    return in;
};

template <typename T>
void search(const TGraph<T> &g, const T &vertex, T &sink, std::map<T, bool> &marked, const std::set<T> &sinks, std::set<T> &unmarkedSinks, std::set<T> &unmarkedSources, bool &sink_not_found)
{
    if (marked[vertex] == false)
    {
        if (sinks.find(vertex) != sinks.end() && sink == T())
        {
            sink = vertex;
            sink_not_found = false;
        }
        
        if (unmarkedSinks.find(vertex) != unmarkedSinks.end())
            unmarkedSinks.erase(unmarkedSinks.find(vertex));
        
        else if (unmarkedSources.find(vertex) != unmarkedSources.end())
            unmarkedSources.erase(unmarkedSources.find(vertex));
        
        marked[vertex] = true;
        
//        std::cout << g << std::endl;
        for (typename TGraph<T>::TConstEdgeIterator edge_it = g.GetVertexNeighboursBegin(vertex); edge_it != g.GetVertexNeighboursEnd(vertex); ++edge_it)
        {
//            if (sink_not_found == true)
            {
                search(g, edge_it->Destination, sink, marked, sinks, unmarkedSinks, unmarkedSources, sink_not_found);
            }
        }
    }
}

template <typename T>
size_t getIndex(const TGraph<T> &g, const std::set<T> &sinks, const std::set<T> &sources, std::vector<T> &sinks_ordered, std::vector<T> &sources_ordered)
{
    int p = -1;
    
    std::map<T, bool> marked;
    
    std::set<T> unmarkedSinks(sinks), unmarkedSources(sources);
    
    while (unmarkedSinks.size())
    {
        T unmarked_source = *unmarkedSources.begin();
        T sink = T();
        bool sink_not_found = true;
        search(g, unmarked_source, sink, marked, sinks, unmarkedSinks, unmarkedSources, sink_not_found);
        
        if (sink != T())
        {
            p++;
            std::iter_swap(sources_ordered.begin() + p, std::find(sources_ordered.begin(), sources_ordered.end(), unmarked_source));
            std::iter_swap(sinks_ordered.begin() + p, std::find(sinks_ordered.begin(), sinks_ordered.end(), sink));
        }
    }
    
    return p;
}

template <typename T>
std::set<std::pair<T, T> > upgradingToSCG(const TGraph<T> &g)
{
    std::set<T> sinks, sources;
    std::set<std::pair<T, T> > edges;
    
    for (typename TGraph<T>::TConstVertexIterator vertex_it = g.GetVerticesBegin(); vertex_it != g.GetVerticesEnd(); ++vertex_it)
    {
        sinks.insert(*vertex_it);
        sources.insert(*vertex_it);
    }
    
    for (typename TGraph<T>::TConstVertexIterator vertex_it = g.GetVerticesBegin(); vertex_it != g.GetVerticesEnd(); ++vertex_it)
    {
        if (g.GetVertexNeighboursBegin(*vertex_it) != g.GetVertexNeighboursEnd(*vertex_it))
        {
            if (sinks.find(*vertex_it) != sinks.end())
                sinks.erase(sinks.find(*vertex_it));
            
            for (typename TGraph<T>::TConstEdgeIterator edge_it = g.GetVertexNeighboursBegin(*vertex_it); edge_it != g.GetVertexNeighboursEnd(*vertex_it); ++edge_it)
            {
                if (sources.find(edge_it->Destination) != sources.end())
                    sources.erase(sources.find(edge_it->Destination));
            }
        }
        else
        {
            if (sources.find(*vertex_it) != sources.end())
                sources.erase(sources.find(*vertex_it));
        }
    }
    
    std::vector<T> sinks_ordered;
    std::vector<T> sources_ordered(sources.size());
    
    for (typename std::set<T>::const_iterator it = sinks.begin(); it != sinks.end(); ++it)
    {
        sinks_ordered.push_back(*it);
    }
    
    for (typename std::set<T>::const_iterator it = sources.begin(); it != sources.end(); ++it)
    {
        sources_ordered.push_back(*it);
    }
    
    size_t p = getIndex(g, sinks, sources, sinks_ordered, sources_ordered);
    
    std::cout << sinks_ordered << std::endl;
    std::cout << sources_ordered << std::endl;
    std::cout << p << std::endl;
    
    return edges;
}

template <typename T>
void work(const TGraph<T> &g)
{
    typedef typename TGraph<T>::condensed_type condensed_type;
    
    TGraph<condensed_type> condensation = g.condensation();
    std::set<std::pair<condensed_type, condensed_type> > edges = upgradingToSCG(condensation);
}

int main()
{
    TGraph<int> g;
    std::fstream fstream("input.txt");
    
    fstream >> g;
    work(g);
    
    return 0;
}
