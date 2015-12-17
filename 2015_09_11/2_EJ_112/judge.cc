#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <exception>

template <typename T>
std::ostream &operator <<(std::ostream &os, const std::pair<T, T> &pair)
{
    os << "(" << pair.first << ", " << pair.second << ")";
    return os;
}

#include <iostream>
#include <set>
#include <map>

template<typename TNode>
class TGraph
{
    
private:
    
    typedef std::set<std::pair<TNode, float> > TAdjValue;
    typedef std::map<TNode, TAdjValue> TAdjacencies;
    TAdjacencies Adjacencies;
    
public:
    
    inline size_t getVertexCount() const;
    
    TGraph();
    TGraph(const TGraph &);
    //explicit TGraph(const std::vector<std::pair<TNode, TNode> >& edges);
    
    class TConstVertexIterator
    {
        
    private:
        
        typename TAdjacencies::const_iterator InternalIt;
        
    public:
        
        TConstVertexIterator(const TConstVertexIterator&);
        TConstVertexIterator &operator = (const TConstVertexIterator&);
        TConstVertexIterator(typename TAdjacencies::const_iterator it);
        
        const TNode& operator*() const;
        const TNode* operator->() const;
        bool operator == (const TConstVertexIterator&) const;
        bool operator != (const TConstVertexIterator&) const;
        TConstVertexIterator& operator++();
        
    };
    
    TConstVertexIterator GetVerticesBegin() const;
    TConstVertexIterator GetVerticesEnd() const;
    
    bool HasVertex(const TNode&) const;
    
    struct TEdge
    {
        TNode Source;
        TNode Destination;
        float weight;
    };
    
    class TConstEdgeIterator
    {
        
    private:
        
        bool isInit;
        typename TAdjValue::const_iterator InternalIt;
        TEdge InternalEdge;
        
        void Initialize();
        
    public:
        
        TConstEdgeIterator(const TNode& source, typename TAdjValue::const_iterator it);
        TConstEdgeIterator(const TConstEdgeIterator&);
        TConstEdgeIterator& operator=(const TConstEdgeIterator&);
        
        const TEdge &operator* ();
        const TEdge *operator-> ();
        bool operator == (const TConstEdgeIterator&) const;
        bool operator != (const TConstEdgeIterator&) const;
        TConstEdgeIterator &operator++ ();
        
    };
    
    TConstEdgeIterator GetVertexNeighboursBegin(const TNode&) const;
    TConstEdgeIterator GetVertexNeighboursEnd(const TNode&) const;
    
    bool AddVertex(const TNode&);
    bool AddEdge(const TNode&, const TNode&, const float weight);
    
    TGraph<TNode> transposedGraph() const;
    
    std::pair<std::map<TNode, float>, std::map<TNode, TNode> > dijkstra(const TNode &sourceVertex) const;
    
    template <typename T>
    friend std::ostream &operator <<(std::ostream &os, const TGraph<T> &g);
};

template <typename TNode>
TGraph<TNode>::TGraph()
{
    
}

template <typename TNode>
TGraph<TNode>::TGraph(const TGraph<TNode>& g): Adjacencies(g.Adjacencies)
{
    
}

template <typename TNode>
inline size_t TGraph<TNode>::getVertexCount() const
{
    return Adjacencies.size();
}

template <typename TNode>
bool TGraph<TNode>::AddEdge(const TNode &src, const TNode &dst, const float weight)
{
    typename std::map<TNode, TAdjValue>::iterator srcIt;
    if ((srcIt = Adjacencies.find(src)) == Adjacencies.end())
        return false;
    
    if (srcIt->second.find(std::make_pair(dst, weight)) != srcIt->second.end())
        return false;
    
    srcIt->second.insert(std::make_pair(dst, weight));
    
    return true;
}

template <typename TNode>
bool TGraph<TNode>::AddVertex(const TNode &node)
{
    if(Adjacencies.find(node) != Adjacencies.end())
        return false;
    
    Adjacencies[node] = TAdjValue();
    
    return true;
}

#pragma mark - VertexIterator

template <typename TNode>
TGraph<TNode>::TConstVertexIterator::TConstVertexIterator(typename TGraph<TNode>::TAdjacencies::const_iterator it): InternalIt(it)
{
    
}

template <typename TNode>
TGraph<TNode>::TConstVertexIterator::TConstVertexIterator(const TGraph<TNode>::TConstVertexIterator &rhs): InternalIt(rhs.InternalIt)
{
    
}

template<typename TNode>
typename TGraph<TNode>::TConstVertexIterator &TGraph<TNode>::TConstVertexIterator::operator= (const TGraph<TNode>::TConstVertexIterator& rhs)
{
    InternalIt = rhs.InternalIt;
    return *this;
}

template<typename TNode>
const TNode& TGraph<TNode>::TConstVertexIterator::operator*() const
{
    return InternalIt->first;
}

template<typename TNode>
const TNode* TGraph<TNode>::TConstVertexIterator::operator->() const
{
    return &(InternalIt->first);
}

template<typename TNode>
bool TGraph<TNode>::TConstVertexIterator::operator==(const TGraph<TNode>::TConstVertexIterator& rhs) const
{
    return InternalIt == rhs.InternalIt;
}

template<typename TNode>
bool TGraph<TNode>::TConstVertexIterator::operator!=(const TGraph<TNode>::TConstVertexIterator& rhs) const
{
    return InternalIt != rhs.InternalIt;
}

template<typename TNode>
typename TGraph<TNode>::TConstVertexIterator &TGraph<TNode>::TConstVertexIterator::operator++ ()
{
    ++InternalIt;
    return *this;
}

template<typename TNode>
typename TGraph<TNode>::TConstVertexIterator TGraph<TNode>::GetVerticesBegin() const
{
    return TConstVertexIterator(Adjacencies.begin());
}

template<typename TNode>
typename TGraph<TNode>::TConstVertexIterator TGraph<TNode>::GetVerticesEnd() const
{
    return TConstVertexIterator(Adjacencies.end());
}

#pragma mark - EdgeIterator

template<typename TNode>
void TGraph<TNode>::TConstEdgeIterator::Initialize()
{
    if (!isInit)
    {
        isInit = true;
        InternalEdge.Destination = InternalIt->first;
        InternalEdge.weight = InternalIt->second;
    }
}

template<typename TNode>
TGraph<TNode>::TConstEdgeIterator::TConstEdgeIterator(const TNode& source, typename TGraph<TNode>::TAdjValue::const_iterator it): InternalIt(it), isInit(false)
{
    InternalEdge.Source = source;
}

template<typename TNode>
TGraph<TNode>::TConstEdgeIterator::TConstEdgeIterator(const TConstEdgeIterator &rhs): InternalIt(rhs.InternalIt), InternalEdge(rhs.InternalEdge)
{
    
}

template<typename TNode>
typename TGraph<TNode>::TConstEdgeIterator &TGraph<TNode>::TConstEdgeIterator::operator=(const TGraph<TNode>::TConstEdgeIterator& rhs)
{
    InternalIt = rhs.InternalIt;
    InternalEdge = rhs.InternalEdge;
    return *this;
}

template<typename TNode>
const typename TGraph<TNode>::TEdge& TGraph<TNode>::TConstEdgeIterator::operator*()
{
    Initialize();
    return InternalEdge;
}

template<typename TNode>
const typename TGraph<TNode>::TEdge* TGraph<TNode>::TConstEdgeIterator::operator->()
{
    Initialize();
    return &InternalEdge;
}

template<typename TNode>
bool TGraph<TNode>::TConstEdgeIterator::operator== (const TGraph<TNode>::TConstEdgeIterator& rhs) const
{
    return InternalIt == rhs.InternalIt;
}

template<typename TNode>
bool TGraph<TNode>::TConstEdgeIterator::operator!= (const TGraph<TNode>::TConstEdgeIterator& rhs) const
{
    return InternalIt != rhs.InternalIt;
}

template<typename TNode>
typename TGraph<TNode>::TConstEdgeIterator &TGraph<TNode>::TConstEdgeIterator::operator++ ()
{
    ++InternalIt;
    isInit = false;
    return *this;
}

template<typename TNode>
typename TGraph<TNode>::TConstEdgeIterator TGraph<TNode>::GetVertexNeighboursBegin(const TNode& v) const
{
    typename std::map<TNode, TAdjValue>::const_iterator it = Adjacencies.find(v);
    if (it != Adjacencies.end())
    {
        return TConstEdgeIterator(v, it->second.begin());
    }
}

template<typename TNode>
typename TGraph<TNode>::TConstEdgeIterator TGraph<TNode>::GetVertexNeighboursEnd(const TNode& v) const
{
    typename std::map<TNode, TAdjValue>::const_iterator it = Adjacencies.find(v);
    if (it != Adjacencies.end())
    {
        return TConstEdgeIterator(v, it->second.end());
    }
}

#pragma mark - Algotithms

template <typename T>
std::pair<std::map<T, float>, std::map<T, T> > TGraph<T>::dijkstra(const T &sourceVertex) const
{
    std::map<T, float> destinations;
    std::map<T, T> paths;
    
    destinations[sourceVertex] = 0;
    std::set<std::pair<float, T> > queue;
    
    for (typename TGraph<T>::TConstVertexIterator vertex = this->GetVerticesBegin(); vertex != this->GetVerticesEnd(); ++vertex)
    {
        if (sourceVertex != *vertex)
            destinations[*vertex] = FLT_MAX / 2;
        
        queue.insert(std::make_pair(destinations[*vertex], *vertex));
    }
    
    while (queue.empty() == false)
    {
        std::pair<float, T> pair = *queue.begin();
        queue.erase(queue.begin());
        
        for (typename TGraph<T>::TConstEdgeIterator edge = this->GetVertexNeighboursBegin(pair.second); edge != this->GetVertexNeighboursEnd(pair.second); ++edge)
        {
            float alternativeWeight = pair.first + edge->weight;
            if (alternativeWeight < destinations[edge->Destination])
            {
                std::pair<float, T> dst = *queue.find(std::make_pair(destinations[edge->Destination], edge->Destination));
                queue.erase(dst);
                
                destinations[edge->Destination] = alternativeWeight;
                dst.first = alternativeWeight;
                
                queue.insert(std::make_pair(destinations[edge->Destination], edge->Destination));
                
                paths[edge->Destination] = pair.second;
            }
        }
    }
    return std::make_pair(destinations, paths);
}

float calculateDistance(const std::pair<int, int> &v1, const std::pair<int, int> &v2)
{
    float distance = 0;
    
    float t1 = powf(v1.first - v2.first, 2);
    float t2 = powf(v1.second - v2.second, 2);
    
    distance = sqrtf(t1 + t2);
    
    return distance;
}

template <typename T>
void readGraph(std::istream &is, TGraph<T> &g, T &entry, std::vector<T> &exits)
{
    size_t row_qty = 0, column_qty = 0;
    
    if (!(is >> row_qty)){}
    
    if (!(is >> column_qty)){}
    
    if (!(is >> entry.first)){}
    
    if (!(is >> entry.second)){}
    
    entry.first--;
    entry.second--;
    
    std::vector<std::vector<int> > matrix(row_qty, std::vector<int>(column_qty));
    
    for (size_t row = 0; row < row_qty; row++)
    {
        for (size_t column = 0; column < column_qty; column++)
        {
            if (!(is >> matrix[row][column])){}
        }
    }
    
    for (size_t row = 0; row < row_qty; row++)
    {
        for (size_t column = 0; column < column_qty; column++)
        {
            if (matrix[row][column] == 0)
            {
                g.AddVertex(std::make_pair(row, column));
                
                if (row != row_qty - 1 && matrix[row + 1][column] == 0)
                {
                    g.AddVertex(std::make_pair(row + 1, column));
                    g.AddEdge(std::make_pair(row + 1, column), std::make_pair(row, column), 1);
                    g.AddEdge(std::make_pair(row, column), std::make_pair(row + 1, column), 1);
                }
                
                if (column != column_qty - 1 && matrix[row][column + 1] == 0)
                {
                    g.AddVertex(std::make_pair(row, column + 1));
                    g.AddEdge(std::make_pair(row, column), std::make_pair(row, column + 1), 1);
                    g.AddEdge(std::make_pair(row, column + 1), std::make_pair(row, column), 1);
                }
                
                if (row > 0 && matrix[row - 1][column] == 0)
                {
                    g.AddVertex(std::make_pair(row - 1, column));
                    g.AddEdge(std::make_pair(row - 1, column), std::make_pair(row, column), 1);
                    g.AddEdge(std::make_pair(row, column), std::make_pair(row - 1, column), 1);
                }
                
                if (column > 0 && matrix[row][column - 1] == 0)
                {
                    g.AddVertex(std::make_pair(row, column - 1));
                    g.AddEdge(std::make_pair(row, column), std::make_pair(row, column - 1), 1);
                    g.AddEdge(std::make_pair(row, column - 1), std::make_pair(row, column), 1);
                }
            }
        }
    }
    
    std::vector<std::pair<T, T> > hyperToonels;
    size_t toonels_qty = 0;
    if (!(is >> toonels_qty)){}
    
    for (size_t index = 0; index < toonels_qty; index++)
    {
        T entry, exit;
        
        if (!(is >> entry.first)){}
        
        if (!(is >> entry.second)){}
        
        if (!(is >> exit.first)){}
        
        if (!(is >> exit.second)){}
        
        entry.first--;
        entry.second--;
        exit.first--;
        exit.second--;
        
        g.AddVertex(entry);
        g.AddVertex(exit);
        
        g.AddEdge(entry, exit, 0);
        
        hyperToonels.push_back(std::make_pair(entry, exit));
    }
    
    size_t exits_qty = 0;
    if (!(is >> exits_qty)){}
    
    for (size_t index = 0; index < exits_qty; index++)
    {
        T exit;
        
        if (!(is >> exit.first)){}
        
        if (!(is >> exit.second)){}
        
        exit.first--;
        exit.second--;
        
        exits.push_back(exit);
    }
}


int main(int argc, const char * argv[])
{
    typedef std::pair<int, int> T;
    TGraph<T> g;
    
    T entry;
    std::vector<T> exits;
    
    readGraph(std::cin, g, entry, exits);
    
    std::pair<std::map<T, float>, std::map<T, T> > pair = g.dijkstra(entry);
    
    std::map<T, float> weights = pair.first;
    std::map<T, T> paths = pair.second;
    
    T nearestExit = *exits.begin();
    for (std::vector<T>::const_iterator exit = exits.begin(); exit != exits.end(); ++exit)
    {
        if (weights[nearestExit] > weights[*exit])
            nearestExit = *exit;
    }
    
    if (weights[nearestExit] > FLT_MAX/4)
    {
        std::cout << "Impossible";
        return 0;
    }
    
    std::vector<T> path;
    
    T position = nearestExit;
    while (position != entry)
    {
        path.push_back(position);
        position = paths[position];
    }
    
    std::cout << path.size() + 1 << std::endl << entry.first + 1 << " " << entry.second + 1 << std::endl;
    for (std::vector<T>::reverse_iterator position = path.rbegin(); position != path.rend(); ++position)
    {
        std::cout << position->first + 1 << " " << position->second + 1 << std::endl;
    }
    
    return 0;
}
