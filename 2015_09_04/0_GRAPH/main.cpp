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
void dfs(std::ostream &out, const graph<T> &g)
{
    std::map<T, std::pair<size_t, size_t>> sch =  g.DFS();
    for (auto pair = sch.begin(); pair != sch.end(); ++pair)
    {
        out << pair->first << " " << pair->second.first << " " << pair->second.second << std::endl;
    }
}

template <typename TNode>
void DFSVisit(const graph<TNode> &g, const TNode &vertex, size_t &time, std::map<TNode, std::pair<size_t, size_t>> &schedule)
{
    schedule[vertex] = std::make_pair(time, 0);
    time++;
    
    for (auto eIt = g.begin(vertex), eEnd = g.end(vertex); eIt != eEnd; ++eIt)
    {
        if (!schedule.count(eIt->Destination) && schedule[eIt->Destination].second == 0)
        {
            DFSVisit(g, eIt->Destination, time, schedule);
        }
    }
    
    schedule[vertex] = std::make_pair(schedule[vertex].first, time);
    time++;
}

template <typename TNode>
std::map<TNode, std::pair<size_t, size_t>> DFS(const graph<TNode> &g)
{
    std::map<TNode, std::pair<size_t, size_t>> output;
    size_t time = 1;
    
    for (auto eIt = g.begin(), eEnd = g.end(); eIt != eEnd; ++eIt)
    {
        if (!output.count(*eIt))
        {
            DFSVisit(g, *eIt, time, output);
        }
    }
    
    return output;
}

template <typename TNode>
void DFSVisit(const graph<TNode> &g, const TNode &vertex, std::vector<TNode> &comp, std::map<TNode, bool> &used)
{
    used[vertex] = true;
    comp.push_back(vertex);
    for (auto eIt = g.begin(vertex), eEnd = g.end(vertex); eIt != eEnd; ++eIt)
    {
        if (!used[eIt->Destination])
        {
            DFSVisit(g, eIt->Destination, comp, used);
        }
    }
}

template <typename TNode>
std::vector<std::vector<TNode>> StronglyConnectedComponents(const graph<TNode> &g)
{
    std::vector<std::vector<TNode>> stronglyConnectedComponents;
    
    std::map<TNode, std::pair<size_t, size_t>> schedule = DFS(g);
    graph<TNode> transposedGraph = g.transposedGraph();
    
    std::map<TNode, bool> used;
    
    while (schedule.size())
    {
        std::vector<TNode> comp;
        
        size_t max = 0;
        typename std::map<TNode, std::pair<size_t, size_t>>::const_iterator maxIt = schedule.begin();
        
        for (typename std::map<TNode, std::pair<size_t, size_t>>::const_iterator sIt = schedule.begin(), sItEnd = schedule.end(); sIt != sItEnd; sIt++)
        {
            if (max < sIt->second.second)
            {
                maxIt = sIt;
                max = sIt->second.second;
            }
        }
        
        if (!used[maxIt->first])
        {
            DFSVisit(transposedGraph, maxIt->first, comp, used);
        }
        
        if (comp.empty() == false)
        {
            stronglyConnectedComponents.push_back(comp);
        }
        
        schedule.erase(maxIt);
    }
    return stronglyConnectedComponents;
}

template <typename T>
void sccCosarajy(std::ostream &out, const graph<T> &g)
{
    std::vector<std::vector<T>> scc = StronglyConnectedComponents(g);
    for (auto component: scc)
    {
        for (auto vertex: component)
        {
            out << vertex << " ";
        }
        out << std::endl;
    }
}

template <typename T>
void sccTarjan(std::ostream &out, const graph<T> &g)
{
    std::set<std::set<T>> scc = g.SCC();
    for (auto component: scc)
    {
        for (auto vertex: component)
        {
            out << vertex << " ";
        }
        out << std::endl;
    }
}

template <typename T>
void condensed(std::ostream &out, const graph<T> &g)
{
    graph<std::set<T> > condensation = g.condensation();
    out << condensation;
}

template <typename T>
void work()
{
    std::string input;
    std::cin >> input;

    graph<T> g;
    std::cin >> g;
    
    if (input == "dfs")
    {
        dfs(std::cout, g);
    }
    else if (input == "scc")
    {
        std::string input;
        std::cin >> input;
        
        if (input == "c")
            sccCosarajy(std::cout, g);
        else
            sccTarjan(std::cout, g);
    }
    else if (input == "condensed")
    {
        condensed(std::cout, g);
    }
}

int main()
{
    work<std::string>();
    return 0;
}
