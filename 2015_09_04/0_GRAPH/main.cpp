#include <iostream>
#include <string>
#include <set>
#include <map>

template <typename T, typename K>
std::ostream &operator <<(std::ostream &out, const std::pair<T, K> &p)
{
    out << "(" << p.first << ", " << p.second << ")";
    return out;
}

template <typename T>
std::ostream &operator <<(std::ostream &out, const std::set<T> &set)
{
    for (typename std::set<T>::const_iterator it = set.begin(); it != set.end(); ++it)
    {
        out << *it << " ";
    }
    return out;
}

template <typename T, typename K>
std::ostream &operator <<(std::ostream &out, const std::map<T, K> &map)
{
    for (typename std::map<T, K>::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        out << *it << " ";
    }
    return out;
}

#include <Graph.hpp>

template<typename T>
std::istream &operator >>(std::istream &in, graph<T> &g)
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
void work()
{
    std::string input;
    std::cin >> input;

    graph<T> g;
    std::cin >> g;
}

int main()
{
    work<int>();
    return 0;
}
