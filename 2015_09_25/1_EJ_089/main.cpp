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

#include "Worker.cpp"
#include "Grid.cpp"

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
    
//    std::cout << "Yes" << std::endl;
    
    Worker worker(manhattanHeuristic);
    std::cout << worker.solve(grid);
    
    return 0;
}
