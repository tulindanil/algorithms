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

void workOutUpCell(graph<std::pair<int, int> > &g,
                   const std::vector<std::vector<bool> > &deck, 
                   const size_t size, 
                   const std::pair<int, int> &currentCell)
{
    size_t row = currentCell.first, column = currentCell.second;
    if (row == 0 || deck.at(row - 1).at(column) == 1)
        return;

    std::pair<int, int> upCell = std::make_pair(row - 1, column);
    g.AddEdge(currentCell, upCell, 1);
}

void workOutDownCell(graph<std::pair<int, int> > &g,
                     const std::vector<std::vector<bool> > &deck, 
                     const size_t size, 
                     const std::pair<int, int> &currentCell)
{
    size_t row = currentCell.first, column = currentCell.second;
    if (row == size - 1 || deck.at(row + 1).at(column) == 1)
        return;

    std::pair<int, int> downCell = std::make_pair(row + 1, column);
    g.AddVertex(downCell);
    g.AddEdge(currentCell, downCell, 1);
}

void workOutLeftCell(graph<std::pair<int, int> > &g,
                     const std::vector<std::vector<bool> > &deck, 
                     const size_t size, 
                     const std::pair<int, int> &currentCell)
{
    size_t row = currentCell.first, column = currentCell.second;
    if (column == 0 || deck.at(row).at(column - 1) == 1)
        return;

    std::pair<int, int> leftCell = std::make_pair(row, column - 1);
    g.AddEdge(currentCell, leftCell, 1);
}

void workOutRightCell(graph<std::pair<int, int> > &g,
                      const std::vector<std::vector<bool> > &deck, 
                      const size_t size, 
                      const std::pair<int, int> &currentCell)
{
    size_t row = currentCell.first, column = currentCell.second;
    if (column == size - 1 || deck.at(row).at(column + 1) == 1)
        return;

    std::pair<int, int> rightCell = std::make_pair(row, column + 1);
    g.AddVertex(rightCell);
    g.AddEdge(currentCell, rightCell, 1);
}

void readGraph(std::istream &is,
               graph<std::pair<int, int> > &g, 
               const std::pair<int, int> &source, 
               const std::pair<int, int> &target)
{
    size_t size = 0;
    is >> size;

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
            if (deck[row][column] == 1)
                continue;

            std::pair<int, int> cell = std::make_pair(row, column);

            g.AddVertex(cell);

            bool isWhite = ((row % 2 == 0) && (column % 2 == 0)) ||
                           ((row % 2 == 1) && (column % 2 == 1));

            bool isBlack = !isWhite;

//            bool isBlack = ((row % 2 == 0) && (column % 2 == 1)) ||
//                           ((row % 2 == 1) && (column % 2 == 0));

            if (isWhite == true)
            {
                g.AddEdge(source, cell, 1);

                workOutUpCell(g, deck, size, cell);
                workOutLeftCell(g, deck, size, cell);
                workOutRightCell(g, deck, size, cell);
                workOutDownCell(g, deck, size, cell);
            }

            if (isBlack == true)
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
