#include <iostream>
#include <string>
#include <map>
#include <set>
#include <cassert>
#include <algorithm>
#include <vector>

template <typename T, typename K>
std::ostream &operator <<(std::ostream &os, const std::pair<T, K> &p)
{
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

template <typename T, typename K>
std::ostream &operator <<(std::ostream &os, std::map<T, K> &map)
{
    for (typename std::map<T, K>::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        os << it->first << ", " << it->second << "; "; 
    }
    return os;
}

template <typename T>
std::ostream &operator <<(std::ostream &os, std::vector<T> &v)
{
    for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        os << *it << " "; 
    }
    return os;
}

#include "Graph.hpp"
#include "MaxFlow.cpp"

template <typename T>
std::istream &operator>>(std::istream &is, graph<T> &g)
{
    size_t edges_qty;
    is >> edges_qty >> edges_qty;
    for (size_t index = 0; index < edges_qty; ++index)
    {
        T src, dst; 
        float weight;

        is >> src >> dst >> weight;

        g.AddVertex(src);
        g.AddVertex(dst);
        g.AddEdge(src, dst, weight);
    }

    return is;
}

int main()
{
    graph<int> g;
    std::cin >> g;
    int src = 1, trg = g.size();

    std::cout << maxFlow(g, src, trg) << std::endl;

    return 0;
}
