#ifndef GRID_H
#define GRID_H

#include <math/Vec2.h>

#include <vector>

class Grid {
public:
    typedef std::vector<cocos2d::Vec2> Locations;

    Grid();

    Grid(cocos2d::Vec2 size);

    bool create(cocos2d::Vec2 size, int value = 0);

    Locations getNeighbours(cocos2d::Vec2 node) const;

    void fill(int value);

    void clear() {
        _grid.clear();
    }

    void setNode(cocos2d::Vec2 node, int value) {
        _grid[node.x][node.y] = value;
    }

    int getNode(cocos2d::Vec2 node) const {
        return _grid[node.x][node.y];
    }

    cocos2d::Vec2 getSize() const;

    bool isBounded(cocos2d::Vec2 node) const;

    void print();

private:
    std::vector<std::vector<int>> _grid;
};

#endif // GRID_H
