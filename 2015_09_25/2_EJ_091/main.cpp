#include <iostream>
#include <vector>

#include "Grid.cpp"
#include "Worker.cpp"

std::ostream &operator <<(std::ostream &out, const Move &movement)
{
    switch (movement.direction) {
        case up:
            out << "u";
            break;
            
        case down:
            out << "d";
            break;
            
        case right:
            out << "r";
            break;
            
        case left:
            out << "l";
            break;
            
        default:
            abort();
            break;
    }
    
    return out;
}

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
        os << *it;
    }
    return os;
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
                distance += 16*(abs(sx - static_cast<int>(x)) + abs(sy - static_cast<int>(y)));
            }
        }
    }
    return distance;
}

int main(int argc, const char * argv[])
{
    Grid grid(4);
    std::cin >> grid;
    
    if (Worker::isSolvable(grid) == false)
    {
        std::cout << "NO" << std::endl;
        return 0;
    }
    
    Worker worker(manhattanHeuristic);
    std::cout << worker.solve(grid);
    
    return 0;
}
