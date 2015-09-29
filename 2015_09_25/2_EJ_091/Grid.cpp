#include "Grid.hpp"

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