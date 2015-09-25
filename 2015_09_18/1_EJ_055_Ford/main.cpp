#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>

#include "Graph.hpp"

template <typename T>
std::istream &operator >>(std::istream &in, std::pair<T, T> &p)
{
    in >> p.first;
    in >> p.second;
    return in;
}

template <typename T>
std::map<T, float> shortestPath(const TGraph<T> &g, const T &source)
{
    std::map<T, float> distance;
    for (typename TGraph<T>::TConstVertexIterator vertex = g.GetVerticesBegin(); vertex != g.GetVerticesEnd(); ++vertex)
    {
        if (*vertex == source)
            distance[*vertex] = 0;
        else
            distance[*vertex] = FLT_MAX/2;
    }
    
    for (int i = 1; i < g.size(); i++)
    {
        for (typename TGraph<T>::TConstVertexIterator vertex = g.GetVerticesBegin(); vertex != g.GetVerticesEnd(); ++vertex)
        {
            for (typename TGraph<T>::TConstEdgeIterator edge = g.GetVertexNeighboursBegin(*vertex); edge != g.GetVertexNeighboursEnd(*vertex); ++edge)
            {
                if (distance[edge->Destination] + edge->weight < distance[*vertex])
                    distance[*vertex] = distance[edge->Destination] + edge->weight;
            }
        }
    }
    return distance;
}

int main()
{
    
    size_t qty = 0;
    std::cin >> qty;
    
    float distance = 0;
    std::cin >> distance;
    
    std::pair<float, float> A, B;
    std::cin >> A >> B;
    
    std::vector<std::pair<float, float> > cities, abroad;
    
    for (size_t i = 2; i < qty; i++)
    {
        std::pair<float, float> p;
        std::cin >> p;
        
        if (p.first < 0)
            abroad.push_back(p);
        
        cities.push_back(p);
    }
    
    cities.push_back(A);
    cities.push_back(B);
    
    TGraph<std::pair<float, float> > g;
    
    for (std::vector<std::pair<float, float> >::const_iterator p1 = cities.begin(); p1 != cities.end(); ++p1)
    {
        g.AddVertex(*p1);
        for (std::vector<std::pair<float, float> >::const_iterator p2 = cities.begin(); p2 != cities.end(); ++p2)
        {
            if (*p1 == *p2)
                continue;
            
            g.AddVertex(*p2);
            
            float dist = sqrt(pow(p1->first - p2->first, 2) + pow(p1->second - p2->second, 2));
            if (dist < (distance))
            {
                g.AddEdge(*p1, *p2, (dist));
                g.AddEdge(*p2, *p1, (dist));
            }
        }
    }
    
    float min = FLT_MAX;
    for (std::vector<std::pair<float, float> >::const_iterator c = abroad.begin(); c != abroad.end(); ++c)
    {
        float fst = shortestPath(g, A)[*c], snd = shortestPath(g, *c)[B];
        if (min > fst + snd)
        {
            min = fst + snd;
        }
    }
    std::cout << min/1.00 << std::endl;
    
    return 0;
}
