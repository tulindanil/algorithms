#include <iostream>
#include <string>
#include <map>
#include <set>
#include <cassert>
#include <algorithm>
#include <vector>

#include "MaxFlow.cpp"

struct movement
{
    int dx, dy;
    movement(int dx, int dy): dx(dx), dy(dy) { }
};

template<class W>
class worker {
    
public:

    static void solve(std::istream &is,
                      std::ostream& os) {
        worker<W> w;
        w.run(is, os); 
    }

private:

    typedef int T;
    typedef std::vector<W> row;
    typedef std::vector<row> matrix;

    matrix capacity;

    std::vector<std::vector<bool> > deck;
    size_t size;
   
    void run(std::istream &is, 
             std::ostream &os) {
        read(is);
        T src = size*size , trg = size*size + 1;
        os << flow<W>::getMaxFlow(capacity, src, trg) << std::endl;
    }

    inline T getIndex(const std::pair<T, T>& pair) {
        return size*pair.first + pair.second;
    }

    void read(std::istream &is) {
        is >> size;
        capacity = matrix(size*size + 2, row(size*size + 2));
        T src = size*size, dst = size*size + 1; 

        movement moves[] = {movement(-1, 0), movement(1, 0), movement(0, 1), movement(0, -1)};
        deck = std::vector<std::vector<bool> >(size, std::vector<bool>(size));

        for (size_t row = 0; row < size; ++row) {
            std::string s;
            is >> s;
            for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
                deck.at(row).at(it - s.begin()) = (*it == '#') ? true : false;
            }
        }

        for (size_t row = 0; row < size; ++row) {
            for (size_t column = 0; column < size; ++column) {
                if (deck.at(row).at(column) == true)
                    continue;

                std::pair<T, T> cell = std::make_pair(row, column);

                bool isWhite = (row + column) % 2;
                bool isBlack = !isWhite;

                if (isWhite == true) {
                    capacity.at(src).at(getIndex(cell)) = 1;
                    for (size_t i = 0; i < 4; ++i)
                        shift(moves[i], cell);
                }
                else 
                    capacity.at(getIndex(cell)).at(dst) = 1;
            }
        }

    }

    inline void shift(const movement &move,
               const std::pair<T, T> &currentCell) {
        T index = getIndex(currentCell) + 3*move.dx + move.dy;
        
        T row = currentCell.first + move.dx;
        T column = currentCell.second + move.dy;

        if ((row < 0 || row == size) || (column < 0 || column == size) || deck.at(row).at(column) == true)
            return;
        capacity.at(getIndex(currentCell)).at(index) = 1;
    }
};

template<class W>
void work() {
    worker<W>::solve(std::cin,
                     std::cout);
}

int main() {
    work<int>();
    return 0;
}
