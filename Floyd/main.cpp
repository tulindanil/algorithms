#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

#include <Graph.hpp>

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
std::ostream &operator <<(std::ostream &out, std::vector<std::vector<T>> &matrix)
{
    for (auto i: matrix)
    {
        for (auto j: i)
        {
            out << ((j != FLT_MAX) ? j : 0) << " ";
        }
        out << std::endl;
    }
    
    return out;
}

template <typename T>
void work(const TGraph<T> &g)
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
            matrix[*vertex][edge->Destination] = edge->weight;
        }
    }
    
    std::cout << matrix;
    
    for (size_t k = 0; k < g.size(); ++k)
    {
        for (size_t i = 0; i < g.size(); ++i)
        {
            for (size_t j = 0; j < g.size(); ++j)
            {
                matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);
            }
        }
    }
    
    std::cout << matrix;
}

int main()
{
    std::fstream f("input.txt");
    
    TGraph<int> g;
    
    f >> g;
    work(g);
    
    return 0;
}
