#include <iostream>
#include <vector>

template <typename T, typename V>
std::ostream &operator <<(std::ostream &os, const std::pair<T, V> &pair)
{
    os << "(" << pair.first << ", " << pair.second << ")";
    return os;
}

template <typename T>
std::ostream &operator <<(std::ostream &os, const std::vector<T> &vector)
{
    for (typename std::vector<T>::const_iterator it = vector.begin(); it != vector.end(); ++it)
    {
        os << *it << ", ";
    }
    return os;
}


#ifndef __Worker__
#define __Worker__


#ifndef __Grid__
#define __Grid__

#include <iostream>
#include <vector>

class Grid
{
    
    size_t _size;
    size_t emptyCellPosition;
    std::vector<size_t> content;
    
    void findEmptyCell();
    bool isPosValid(int x, int y) const;
    
public:
    
    Grid(const size_t size);
    size_t size() const;
    
    bool move(int dx, int dy);
    
    size_t operator [](const size_t index) const;
    size_t &operator [](const size_t index);
    
    size_t &at(size_t, size_t);
    size_t at(size_t, size_t) const;
    
    bool operator <(const Grid &rhs) const;
    
    friend std::istream &operator >>(std::istream &in, Grid &grid);
    
};

#endif

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
#include <cassert>

Move movements[] =
{
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
        
        if (hasReachedGoal)
        {
            target = v.grid;
            std::cout << "Yes" << std::endl;
            break;
        }
        
        if (hasReachedDepthLimit)
            continue;
        
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

#ifndef __Grid__
#define __Grid__

#include <iostream>
#include <vector>

class Grid
{
    
    size_t _size;
    size_t emptyCellPosition;
    std::vector<size_t> content;
    
    void findEmptyCell();
    bool isPosValid(int x, int y) const;
    
public:
    
    Grid(const size_t size);
    size_t size() const;
    
    bool move(int dx, int dy);
    
    size_t operator [](const size_t index) const;
    size_t &operator [](const size_t index);
    
    size_t &at(size_t, size_t);
    size_t at(size_t, size_t) const;
    
    bool operator <(const Grid &rhs) const;
    
    friend std::istream &operator >>(std::istream &in, Grid &grid);
    
};

#endif

#include <iostream>
#include <algorithm>
#include <cassert>

Grid::Grid(const size_t size): _size(size)
{
    
}

size_t Grid::operator[](const size_t index) const
{
    assert(index < content.size());
    return content.at(index);
}

void Grid::findEmptyCell()
{
    emptyCellPosition = std::find(content.begin(), content.end(), 0) - content.begin();
}

bool Grid::isPosValid(int x, int y) const
{
    return 0 <= x && x < _size &&
            0 <= y && y < _size;
}

bool Grid::move(int dx, int dy)
{
    findEmptyCell();
    
    std::pair<int, int> emptyCellPos = std::make_pair(emptyCellPosition / _size, emptyCellPosition % _size);
    std::pair<int, int> newEmptyCellPos = std::make_pair(emptyCellPosition / _size + dx, emptyCellPosition % _size + dy);
    
    if (isPosValid(newEmptyCellPos.first, newEmptyCellPos.second))
    {
        std::swap(at(newEmptyCellPos.first, newEmptyCellPos.second), at(emptyCellPos.first, emptyCellPos.second));
        
        findEmptyCell();
        assert(emptyCellPosition == newEmptyCellPos.first * _size + newEmptyCellPos.second);
        return true;
    }
    return false;
}

size_t &Grid::operator[](const size_t index)
{
    assert(index < content.size());
    return content.at(index);
}

size_t &Grid::at(size_t fstIndex, size_t sndIndex)
{
    assert(_size * fstIndex + sndIndex < _size * _size);
    return content[_size * fstIndex + sndIndex];
}

size_t Grid::at(size_t fstIndex, size_t sndIndex) const
{
    assert(_size * fstIndex + sndIndex < _size * _size);
    return content[_size * fstIndex + sndIndex];
}

size_t Grid::size() const
{
    return _size;
}

bool Grid::operator <(const Grid &grid) const
{
    return content < grid.content;
}

std::istream &operator >>(std::istream &in, Grid &grid)
{
    size_t size = grid.size() * grid.size();
    grid.content = std::vector<size_t>(size, 9);
    for (size_t index = 0; index < grid.size() * grid.size(); ++index)
    {
        in >> grid.content[index];
    }
    grid.findEmptyCell();
    return in;
}
size_t manhattanHeuristic(const Grid& grid)
{
    int distance = 0;
    for (size_t x = 0; x < grid.size(); ++x) {
        for (size_t y = 0; y < grid.size(); ++y) {
            size_t val = grid.at(x, y);
            if (val) {
                int sx = static_cast<int>((val - 1) / grid.size());
                int sy = static_cast<int>((val - 1) % grid.size());
                distance += (abs(sx - static_cast<int>(x)) + abs(sy - static_cast<int>(y)));
            }
        }
    }
    return distance;
}

int main(int argc, const char * argv[])
{
    Grid grid(3);
    std::cin >> grid;
    
    if (Worker::isSolvable(grid) == false)
    {
        std::cout << "No" << std::endl;
        return 0;
    }
    
    Worker worker(manhattanHeuristic);
    std::cout << worker.solve(grid);
    
    return 0;
}
