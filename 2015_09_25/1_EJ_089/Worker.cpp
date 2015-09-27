#include "Worker.hpp"
#include <cassert>

Move movements[] = {
    {0, 1,  right},
    {1, 0,  down},
    {0, -1, left},
    {-1, 0, up}
};

inline void Worker::relaxState(const Grid& grid, size_t newDist, Move newMove, int depth)
{
    bool notVisited = !distance.count(grid);
    bool betterDist = (newDist < distance[grid]);
    
    if (notVisited || betterDist)
    {
        if (!notVisited)
        {
            OrderedState st;
            st.distFromStart = distance[grid];
            st.distToEnd = heuristic(grid);
            st.grid = grid;
            st.depth = -1;
            
            std::set<OrderedState>::const_iterator it = stQueue.lower_bound(st);
            assert(it != stQueue.end());
            stQueue.erase(it);
        }
        
        moveMap[grid] = newMove;
        distance[grid] = newDist;
        
        // insert new data
        
        OrderedState st;
        
        st.distFromStart = distance[grid];
        st.distToEnd = heuristic(grid);
        st.grid = grid;
        st.depth = depth;
        
        stQueue.insert(st);
    }
}

inline void Worker::traverseNeighbours(OrderedState& v)
{
    Grid grid = v.grid;
    
    for (size_t index = 0; index < 4; index++)
    {
        Move movement = movements[index];
        bool hasMoved = grid.move(movement.x, movement.y);
        if (hasMoved)
        {
            relaxState(grid, v.distFromStart + 1, movement, v.depth + 1);
            bool hasMovedBack = grid.move(-movement.x, -movement.y);
            assert(hasMovedBack);
        }
    }
}

size_t Worker::restorePath()
{
    Grid v = target;
    size_t length = 0;
    
    if (distance.find(v) == distance.end())
    {
        std::cout << "No" << std::endl;
        exit(1);
    }
    
    while (distance[v])
    {
        length++;
        assert(v.move(-moveMap[v].x, -moveMap[v].y));
    }
    return length;
}

size_t Worker::solve(const Grid &source)
{
    relaxState(source, 0, movements[0], -1);
    
    while (!stQueue.empty())
    {
        OrderedState v = *stQueue.begin();
        stQueue.erase(stQueue.begin());
        
        bool hasReachedGoal = (heuristic(v.grid) == 0);
        bool hasReachedDepthLimit = (v.depth >= 1000);
        
        if (hasReachedGoal) {
            target = v.grid;
            std::cout << "Yes" << std::endl;
            break;
        }
        
        if (hasReachedDepthLimit) {
            continue;
        }
        
        traverseNeighbours(v);
    }
    
    return restorePath();
}

bool Worker::isSolvable(const Grid &grid)
{
    size_t inversions = 0;
    size_t cellsQty = grid.size() * grid.size();
    
    for (size_t posOne = 0; posOne < cellsQty; ++posOne)
    {
        size_t firstValue = grid[posOne];
        
        if (firstValue == 0)
            continue;
        
        for (size_t posTwo = posOne + 1; posTwo < cellsQty; ++posTwo)
        {
            size_t secondValue = grid[posTwo];
            if (secondValue > 0)
                inversions += (firstValue < secondValue);
        }
    }
    
    bool inverionsIsEven = (inversions % 2 == 0);
    return (inverionsIsEven);
}
