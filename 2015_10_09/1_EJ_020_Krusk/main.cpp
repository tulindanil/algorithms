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

//#define max(x, y) ((x)>(y) ? (x) : (y))
//#define min(x, y) ((x)<(y) ? (x) : (y))

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
    return fmax(a,c) <= fmin(b,d);
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
void readGraph(std::istream &in, graph<T> &g, std::vector<segment> &ditches)
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

template<typename T>
void out(std::ostream &out, const graph<T> &g)
{
    std::multimap<std::string, std::string> map;
    for (typename graph<T>::const_iterator v1 = g.begin(); v1 != g.end(); ++v1)
    {
        for (typename graph<T>::const_edge_iterator edge = g.begin(*v1); edge != g.end(*v1); ++edge)
        {
            map.insert(std::make_pair(v1->first, edge->Destination.first));
        }
    }
    
    out << map.size() << std::endl;
    for (typename std::multimap<std::string, std::string>::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        out << it->first << " " << it->second << std::endl;
    }
}

const int MAXN = 1000;
int parent[MAXN];

void make_set (int v)
{
    parent[v] = v;
}

int find_set (int v)
{
    if(v == parent[v])
        return v;
    return find_set(parent[v]);
}

void union_sets (int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b)
        parent[b] = a;
}

bool isCycled(int x, int y)
{
    return find_set(x) == find_set(y);
}

template <typename T>
graph<T> minSpanningTree(const graph<T> &g, const T &v0)
{
    graph<T> spanningTree;

    typedef std::pair<float, typename graph<T>::edge> ordered;
    std::set<ordered> queue;
    
    std::map<city, int> numbered;
    
    for (typename graph<T>::const_iterator vertex = g.begin(); vertex != g.end(); ++vertex)
    {
        numbered[*vertex] = numbered.size();
        make_set(numbered[*vertex]);
        for (typename graph<T>::const_edge_iterator edge_it = g.begin(*vertex); edge_it != g.end(*vertex); ++edge_it)
        {
            queue.insert(std::make_pair(edge_it->weight, *edge_it));
        }
    }
    
    for (typename std::set<ordered>::const_iterator ord = queue.begin(); ord != queue.end(); ++ord)
    {
        if (isCycled(numbered[ord->second.Destination], numbered[ord->second.Source]) == false)
        {
            union_sets(numbered[ord->second.Destination], numbered[ord->second.Source]);
            
            spanningTree.AddVertex(ord->second.Source);
            spanningTree.AddVertex(ord->second.Destination);
            spanningTree.AddEdge(ord->second.Source, ord->second.Destination, 1);
        }
    }
    return spanningTree;
}

template <typename T>
void work(graph<T> &g)
{
    std::vector<segment> ditches;
    readGraph(std::cin, g, ditches);
    for (typename graph<T>::const_iterator v1 = g.begin(); v1 != g.end(); ++v1)
    {
        for (typename graph<T>::const_iterator v2 = g.begin(); v2 != g.end(); ++v2)
        {
            if (*v1 == *v2)
                continue;
            
            float path = ::distance(*v1, *v2);
            
            if (ditches.empty() == true)
            {
                g.AddEdge(*v1, *v2, path);
                g.AddEdge(*v2, *v1, path);
            }
            else
            {
                bool isPossible = true;
                for (typename std::vector<segment>::const_iterator s = ditches.begin(); s != ditches.end(); ++s)
                {
                    if (intersect(s->first, s->second, v1->second, v2->second) == true)
                    {
                        isPossible = false;
                    }
                }
                if (isPossible)
                {
                    g.AddEdge(*v1, *v2, path);
                    g.AddEdge(*v2, *v1, path);
                }
            }
        }
    }
    
    if (g.condensation().size() == 1)
    {
        std::cout << "YES" << std::endl;
        graph<T> tree = minSpanningTree(g, *g.begin());
        out(std::cout, tree);
    }
    else
    {
        std::cout << "NO";
    }
}

int main()
{
    graph<city> g;
    work(g);
    return 0;
}