//#include "Graph.hpp"
#include <iostream>
#include <fstream>
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

#include "Graph.hpp"

template <typename T>
std::istream &operator >>(std::istream &in, TGraph<T> &g)
{
    size_t vertices, edges;
    
    if (!(in >> vertices)){}
    
    if (!(in >> edges)){}
    
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
    
    std::map<condensed_type, std::pair<size_t, size_t> > dfs_result = condensation.DFS();
    std::set<std::pair<size_t, condensed_type> > schedule;
    
    for (typename std::map<condensed_type, std::pair<size_t, size_t> >::const_iterator it = dfs_result.begin(); it != dfs_result.end(); ++it)
    {
        schedule.insert(std::make_pair(it->second.second, it->first));
    }
    
    size_t qty = 0;
    while (condensation.size())
    {
        condensed_type vertex = (--schedule.end())->second;
        schedule.erase(--schedule.end());
        
        std::map<condensed_type, condensed_type> bfs_result = condensation.BFS(vertex);
        
        for (typename std::map<condensed_type, condensed_type>::const_iterator reachable_vertex = bfs_result.begin(); reachable_vertex != bfs_result.end(); ++reachable_vertex)
        {
            condensed_type vertex = reachable_vertex->first;
            condensation.DeleteVertex(vertex);
            schedule.erase(std::make_pair(dfs_result[vertex].second, vertex));
        }
        
        qty++;
    }
    std::cout << qty << std::endl;
}

int main()
{
    
    TGraph<int> g;
    
    std::ifstream fstream("input.txt");
    fstream >> g;
    
    work(g);
    
    return 0;
}
