#pragma once

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
