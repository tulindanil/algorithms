#include <iostream>
#include <string>

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
        g.AddEdge(src, dst, 1);
    }
    
    return in;
};

template <typename T>
void dfs(std::ostream &out, const TGraph<T> &g)
{
    
}

template <typename T>
void work()
{
    std::string input;
    std::cin >> input;
    
    TGraph<T> g;
    std::cin >> g;
    
    if (input == "dfs")
    {
        dfs(std::cout, g);
    }
}

int main()
{
    work<std::string>();
    
    std::cout << "yeah" << std::endl;   

    return 0;
}
