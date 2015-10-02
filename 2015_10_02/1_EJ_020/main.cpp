#include <iostream>

template<typename T, typename V>
std::ostream &operator <<(std::ostream &out, const std::pair<T, V> &p)
{
    out << "(" << p.first << ", " << p.second << ")";
    return out;
}

#include "Graph.hpp"
#include <vector>
#include <cmath>
#include <map>
#include <queue>

#define max(x, y) ((x)>(y) ? (x) : (y))

typedef std::pair<int, int> point;
typedef std::pair<point, point> segment;
typedef std::pair<std::string, point> city;

typedef point pt;

inline int area (pt a, pt b, pt c) {
    return (b.first - a.first) * (c.second - a.second) - (b.second - a.second) * (c.first - a.first);
}

inline bool intersect_1 (int a, int b, int c, int d) {
    if (a > b)  std::swap (a, b);
    if (c > d)  std::swap (c, d);
    return max(a,c) <= min(b,d);
}

bool intersect (pt a, pt b, pt c, pt d) {
    return intersect_1 (a.first, b.first, c.first, d.first)
    && intersect_1 (a.second, b.second, c.second, d.second)
    && area(a,b,c) * area(a,b,d) <= 0
    && area(c,d,a) * area(c,d,b) <= 0;
}

std::istream &operator >>(std::istream &in, point &c)
{
    in >> c.first;
    in >> c.second;
    return in;
}

std::istream &operator >>(std::istream &in, segment &s)
{
    in >> s.first;
    in >> s.second;
    return in;
}

std::istream &operator >>(std::istream &in, city &c)
{
    in >> c.first;
    in >> c.second;
    return in;
}

float distance(const city &c1, const city &c2)
{
    return sqrt(pow(c1.second.first - c2.second.first, 2) + pow(c1.second.second - c2.second.second, 2));
}

template <typename T>
void readGraph(std::istream &in, TGraph<T> &g, std::vector<segment> &ditches)
{
    size_t cities, dithes_qty;
    
    if (!(in >> cities))
    {
        abort();
    }
    
    if (!(in >> dithes_qty))
    {
        abort();
    }
    
    for (size_t i = 0; i < cities; i++)
    {
        T src;
        in >> src;
        g.AddVertex(src);
    }
    
    for (size_t i = 0; i < dithes_qty; i++)
    {
        segment s;
        in >> s;
        ditches.push_back(s);
    }
}

template <typename T>
TGraph<T> minSpanningTree(const TGraph<T> &g, const T &v0)
{
    TGraph<T> spanningTree;
    
    typedef std::pair<float, T> ordered;
    std::set<ordered> queue;
    std::map<T, float> dist;
    std::map<T, T> pathMap;
    
    for (typename TGraph<T>::TConstVertexIterator v = g.GetVerticesBegin(); v != g.GetVerticesEnd(); ++v)
    {
        if (*v != v0)
        {
            dist[*v] = 3.40282347e+38F;
            queue.insert(std::make_pair(3.40282347e+38F, *v));
        }
    }
    
    queue.insert(std::make_pair(0, v0));
    dist[v0] = 0;
    
    while (queue.empty() == false)
    {
        T v = queue.begin()->second;
        queue.erase(queue.begin());
        
        spanningTree.AddVertex(v);
        
        if (pathMap.find(v) != pathMap.end())
        {
            spanningTree.AddVertex(pathMap[v]);
            spanningTree.AddEdge(v, pathMap[v], 1);
        }
        
        for (typename TGraph<T>::TConstEdgeIterator edge = g.GetVertexNeighboursBegin(v); edge != g.GetVertexNeighboursEnd(v); ++edge)
        {
            if (edge->weight < dist[edge->Destination] && queue.find(std::make_pair(dist[edge->Destination], edge->Destination)) != queue.end())
            {
                ordered ord = *queue.find(std::make_pair(dist[edge->Destination], edge->Destination));
                queue.erase(ord);
                ord.first = edge->weight;
                queue.insert(ord);
                
                dist[edge->Destination] = edge->weight;
                pathMap[edge->Destination] = v;
            }
        }
    }
    
    return spanningTree;
}

template<typename T>
std::ostream &operator<<(std::ostream &out, const TGraph<T> &g)
{
    std::map<std::string, std::string> map;
    for (typename TGraph<T>::TConstVertexIterator v1 = g.GetVerticesBegin(); v1 != g.GetVerticesEnd(); ++v1)
    {
        if (g.GetVertexNeighboursBegin(*v1) != g.GetVertexNeighboursEnd(*v1))
            map[v1->first] = g.GetVertexNeighboursBegin(*v1)->Destination.first;
    }
    out << map.size() << std::endl;
    
    for (typename std::map<std::string, std::string>::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        out << it->first << " " << it->second << std::endl;
    }
    
    return out;
}

template <typename T>
void work(TGraph<T> &g)
{
    std::vector<segment> ditches;
    readGraph(std::cin, g, ditches);
    for (typename TGraph<T>::TConstVertexIterator v1 = g.GetVerticesBegin(); v1 != g.GetVerticesEnd(); ++v1)
    {
        for (typename TGraph<T>::TConstVertexIterator v2 = g.GetVerticesBegin(); v2 != g.GetVerticesEnd(); ++v2)
        {
            if (*v1 == *v2)
                continue;
            
            for (typename std::vector<segment>::const_iterator s = ditches.begin(); s != ditches.end(); ++s)
            {
                if (intersect(s->first, s->second, v1->second, v2->second) == false)
                {
                    float path = distance(*v1, *v2);
                    g.AddEdge(*v1, *v2, path);
                    g.AddEdge(*v2, *v1, path);
                }
            }
        }
    }
    if (g.condensation().size() == 1)
    {
        std::cout << "YES" << std::endl;
        TGraph<T> tree = minSpanningTree(g, *g.GetVerticesBegin());
        std::cout << tree;
    }
    else
    {
        std::cout << "NO";
    }
}

int main()
{
    TGraph<city> g;
    work(g);
    return 0;
}