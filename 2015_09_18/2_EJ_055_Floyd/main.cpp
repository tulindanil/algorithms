#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>

#include <Graph.hpp>

#define find(x) find_(cities, *x)
#define _find(x) find_(cities, x)

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
        
        float weight;
        if (!(in >> weight))
            throw std::runtime_error("bad graph format: weight");
        g.AddEdge(src, dst, weight);
    }
    
    return in;
};

template <typename T>
std::istream &operator >>(std::istream &in, std::pair<T, T> &p)
{
    in >> p.first;
    in >> p.second;
    return in;
}

int find_( const std::vector<std::pair<float, float>>& where, std::pair<float, float> searchParameter )
{
    for( int i = 0; i < where.size(); i++ ) {
        if( where[i] == searchParameter ) {
            return i;
        }
    }
    return -1;
}

template <typename T>
std::vector<std::vector<float>> work(const TGraph<T> &g, const std::vector<std::pair<float, float> > &cities)
{
    std::vector<std::vector<float>> matrix;
    
    for (size_t i = 0; i < g.size(); ++i)
    {
        matrix.push_back(std::vector<float>());
        for (size_t j = 0; j < g.size(); ++j)
        {
            matrix.at(i).push_back(FLT_MAX);
        }
    }
    
    for (typename TGraph<T>::TConstVertexIterator vertex = g.GetVerticesBegin(); vertex != g.GetVerticesEnd(); ++vertex)
    {
        for (typename TGraph<T>::TConstEdgeIterator edge = g.GetVertexNeighboursBegin(*vertex); edge != g.GetVertexNeighboursEnd(*vertex); ++edge)
        {
            matrix[find_(cities, *vertex)][find_(cities, edge->Destination)] = edge->weight;
        }
    }
    
    for (typename TGraph<T>::TConstVertexIterator k = g.GetVerticesBegin(); k != g.GetVerticesEnd(); ++k)
    {
        for (typename TGraph<T>::TConstVertexIterator i = g.GetVerticesBegin(); i != g.GetVerticesEnd(); ++i)
        {
            for (typename TGraph<T>::TConstVertexIterator j = g.GetVerticesBegin(); j != g.GetVerticesEnd(); ++j)
            {
                matrix[find(i)][find(j)] = min(matrix[find(i)][find(j)], matrix[find(i)][find(k)] + matrix[find(k)][find(j)]);
            }
        }
    }
    
    return matrix;
}

int main()
{
    std::fstream f("input.txt");
    
    size_t qty = 0;
    std::cin >> qty;
    
    float distance = 0;
    std::cin >> distance;
    
    std::pair<float, float> A, B, C;
    std::cin >> A >> B;
    
    std::vector<std::pair<float, float> > cities;
    
    for (size_t i = 2; i < qty; i++)
    {
        std::pair<float, float> p;
        std::cin >> p;
        
        if (p.first < 0 && p.second < 0)
            C = p;
        
        cities.push_back(p);
    }
    
    cities.push_back(A);
    cities.push_back(B);
    
    TGraph<std::pair<float, float> > g;
    
    for (typename std::vector<std::pair<float, float>>::const_iterator p1 = cities.begin(); p1 != cities.end(); ++p1)
    {
        g.AddVertex(*p1);
        for (typename std::vector<std::pair<float, float>>::const_iterator p2 = cities.begin(); p2 != cities.end(); ++p2)
        {
            if (p1 == p2)
                continue;
            
            g.AddVertex(*p2);
            
            float dist = pow(p1->first - p2->first, 2) + pow(p1->second - p2->second, 2);
            if (dist < pow(distance, 2))
            {
                g.AddEdge(*p1, *p2, sqrt(dist));
            }
        }
    }
    
    std::vector<std::vector<float>> m = work(g, cities);
    
    std::cout << m[_find(A)][_find(C)] + m[_find(C)][_find(B)] << std::endl;
    
    return 0;
}
