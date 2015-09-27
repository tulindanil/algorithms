#pragma once

#ifndef __Worker__
#define __Worker__

#include "Grid.hpp"

#include <map>
#include <set>

typedef size_t (*Heuristic)(const Grid &);

struct OrderedState
{
    OrderedState(): grid(Grid(0)) { }
    
    size_t distFromStart;
    size_t distToEnd;
    Grid grid;
    int depth;
};

// This compares distances as
// distance from start + heuristic distance to end
static bool operator <(const OrderedState &lhs, const OrderedState &rhs)
{
    size_t lDistance = lhs.distFromStart + lhs.distToEnd;
    size_t rDistance = rhs.distFromStart + rhs.distToEnd;
    
    if (lDistance != rDistance)
        return lDistance < rDistance;
    return lhs.grid < rhs.grid;
}

typedef enum
{
    up,
    left,
    down,
    right
} Direction;

struct Move
{
    int x;
    int y;
    Direction direction;
};

class Worker
{
    
    inline void relaxState(const Grid& grid, size_t newDist, Move newMove, int depth);
    inline void traverseNeighbours(OrderedState& v);
    
    std::map<Grid, size_t> distance;
    std::map<Grid, Move> moveMap;
    std::set<OrderedState> stQueue;
    
    Grid target;
    size_t restorePath();
    
public:
    
    Heuristic heuristic;
    static bool isSolvable(const Grid &grid);
    
    Worker(Heuristic heuristic): heuristic(heuristic), target(Grid(4)) { }
    
    size_t solve(const Grid &);
    
};

#endif
