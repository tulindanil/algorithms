#include <iostream>
#include <vector>
#include <stack>

#define min(x, y) (x > y) ? y : x

#include <Graph.hpp>

template<typename T>
void readGraph(TGraph<T> &g, std::istream &in)
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
};

template <typename T>
void strong(const TGraph<T> &g, const T &vertex, std::map<T, std::pair<std::pair<size_t, size_t>, bool>> &indexes, std::stack<T> &stack, std::set<std::set<T>> &stronglyConnected)
{
    indexes[vertex] = std::make_pair(std::make_pair(indexes.size(), indexes.size()), true);
    stack.push(vertex);
    
    for (auto edge = g.GetVertexNeighboursBegin(vertex); edge != g.GetVertexNeighboursEnd(vertex); ++edge)
    {
        if (indexes.find(edge->Destination) == indexes.end())
        {
            strong(g, edge->Destination, indexes, stack, stronglyConnected);
            indexes[vertex].first.second = min(indexes[vertex].first.second, indexes[edge->Destination].first.second); // v.lowlink = min(v.lowlink, w.lowlink)
        }
        else if (indexes[edge->Destination].second == true)
        {
            indexes[vertex].first.second = min(indexes[vertex].first.second, indexes[edge->Destination].first.first); // v.lowlink = min(v.lowlink, w.index)
        }
    }
    
    if (indexes[vertex].first.second == indexes[vertex].first.first)
    {
        std::set<T> set;
        T prev;
        do
        {
            if (stack.size() == 0)
                break;
            prev = stack.top();
            stack.pop();
            indexes[prev].second = false;
            set.insert(prev);
            
        } while (prev != vertex);
        stronglyConnected.insert(set);
    }
}

template <typename T>
std::set<std::set<T>> stronglyConnected(const TGraph<T> &g)
{
    std::map<T, std::pair<std::pair<size_t, size_t>, bool>> indexes;
    std::set<std::set<T>> set;
    std::stack<T> stack;
    
    for (typename TGraph<T>::TConstVertexIterator it = g.GetVerticesBegin(), endIt = g.GetVerticesEnd(); it != endIt; ++it)
    {
        if (indexes.find(*it) == indexes.end())
            strong(g, *it, indexes, stack, set);
    }
    
    return set;
}

int main()
{
    freopen("input.txt", "r", stdin);
    
    TGraph<std::string> g;
    readGraph(g, std::cin);
    auto set = stronglyConnected(g);
    
    for (auto one_more_set: set)
    {
        std::cout << one_more_set.size() << std::endl;
        for (auto it: one_more_set)
        {
            std::cout << it << " ";
        }
        std::cout << std::endl;
    }
}



