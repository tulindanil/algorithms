#include <iostream>
#include <string>
#include <map>
#include <set>
#include <cassert>
#include <algorithm>
#include <vector>

template <typename T, typename K>
std::ostream &operator <<(std::ostream &os, const std::pair<T, K> &p)
{
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

template <typename T, typename K>
std::ostream &operator <<(std::ostream &os, std::map<T, K> &map)
{
    for (typename std::map<T, K>::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        os << it->first << ", " << it->second << "; "; 
    }
    return os;
}

template <typename T>
std::ostream &operator <<(std::ostream &os, std::vector<T> &v)
{
    for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        os << *it << " "; 
    }
    return os;
}

#include "Graph.hpp"
#include "MaxFlow.cpp"

struct movement
{
    int dx, dy;
    movement(int dx, int dy): dx(dx), dy(dy) { }
};

void shift(graph<std::pair<int, int> > &g,
                          const std::vector<std::vector<bool> > &deck,
                          const movement &move,
                          const size_t size,
                          const std::pair<int, int> &currentCell)
{
    int row = currentCell.first + move.dx;
    int column = currentCell.second + move.dy;

    if ((row < 0 || row == size) || (column < 0 || column == size) || deck.at(row).at(column) == true)
        return;

    std::pair<int, int> cell = std::make_pair(row, column); 

    g.AddVertex(cell);
    g.AddEdge(currentCell, cell, 1);
}

void readGraph(std::istream &is,
               graph<std::pair<int, int> > &g, 
               const std::pair<int, int> &source, 
               const std::pair<int, int> &target)
{
    size_t size = 0;
    is >> size;

    movement moves[] = {movement(-1, 0), movement(1, 0), movement(0, 1), movement(0, -1)};
    std::vector<std::vector<bool> > deck(size, std::vector<bool>(size, 0));

    for (size_t row = 0; row < size; ++row)
    {
        std::string s;
        is >> s;
        for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
        {
            deck[row][it - s.begin()] = (*it == '#') ? true : false;
        }
    }

    g.AddVertex(source);
    g.AddVertex(target);

    for (size_t row = 0; row < size; ++row)
    {
        for (size_t column = 0; column < size; ++column)
        {
            if (deck[row][column] == true)
                continue;

            std::pair<int, int> cell = std::make_pair(row, column);
            g.AddVertex(cell);

            bool isWhite = (row + column) % 2;
            bool isBlack = !isWhite;

            if (isWhite == true)
            {
                g.AddEdge(source, cell, 1);
                for (size_t i = 0; i < 4; ++i)
                    shift(g, deck, moves[i], size, cell);
            }
            else if (isBlack == true)
                g.AddEdge(cell, target, 1);
        }
    }
}

int main()
{
    graph<std::pair<int, int> > g;
    std::pair<int, int> source = std::make_pair(-1, 0),
                        target = std::make_pair(0, -1);

    readGraph(std::cin, g, source, target);
    std::cout << maxFlow(g, source, target) << std::endl;

    return 0;
}
