#include <iostream>
#include <istream>
#include <set>
#include <map>
#include <vector>

template <typename T, typename K>
std::ostream &operator <<(std::ostream &os, const std::pair<T, K> &pair)
{
    os << "(" << pair.first << ", " << pair.second << ")";
    return os;
}

template <typename T>
std::ostream &operator <<(std::ostream &os, const std::set<T> &set)
{
    for (typename std::set<T>::const_iterator it = set.begin(); it != set.end(); ++it)
    {
        os << *it << ", ";
    }
    return os;
}

template <typename T, typename K>
std::ostream &operator <<(std::ostream &os, const std::map<T, K> &map)
{
    for (typename std::map<T, K>::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        os << it->first << " " << it->second << "; ";
    }
    return os;
}

#include "Graph.hpp"

template <typename T>
std::istream &operator >>(std::istream &in, TGraph<T> &g)
{
    size_t vertices, edges;
    
    if (!(in >> vertices))
    {
        abort();
    }
    
    if (!(in >> edges))
    {
        abort();
    }
    
    for (size_t i = 0; i < edges; i++)
    {
        T src;
        if (!(in >> src)){}
        
        g.AddVertex(src);
        
        T dst;
        if (!(in >> dst)){}
        
        g.AddVertex(dst);
        g.AddEdge(src, dst, 1);
    }
    
    return in;
}

template <typename T>
void work(TGraph<T> &g)
{
    typedef typename TGraph<T>::condensed_type condensed_type;
    TGraph<condensed_type> condensation = g.condensation();
    
    std::set<condensed_type> sources, isolated;
    for (typename TGraph<condensed_type>::TConstVertexIterator vertex_it = condensation.GetVerticesBegin(); vertex_it != condensation.GetVerticesEnd(); ++vertex_it)
    {
        sources.insert(*vertex_it);
        isolated.insert(*vertex_it);
    }
    
    for (typename TGraph<condensed_type>::TConstVertexIterator vertex_it = condensation.GetVerticesBegin(); vertex_it != condensation.GetVerticesEnd(); ++vertex_it)
    {
        if (condensation.GetVertexNeighboursBegin(*vertex_it) == condensation.GetVertexNeighboursEnd(*vertex_it))
            sources.erase(sources.find(*vertex_it));
        
        for (typename TGraph<condensed_type>::TConstVertexIterator v = condensation.GetVerticesBegin(); v != condensation.GetVerticesEnd(); ++v)
        {
            for (typename TGraph<condensed_type>::TConstEdgeIterator edge = condensation.GetVertexNeighboursBegin(*v); edge != condensation.GetVertexNeighboursEnd(*v); ++edge)
            {
                isolated.erase(*vertex_it);
                isolated.erase(edge->Destination);
                
                if (edge->Destination == *vertex_it)
                {
                    if (sources.find(*vertex_it) != sources.end())
                        sources.erase(sources.find(*vertex_it));
                    break;
                }
            }
        }
    }
    std::cout << sources.size() + isolated.size();
}

int main()
{
    
    TGraph<int> g;
    std::cin >> g;
    work(g);
    
    return 0;
}
