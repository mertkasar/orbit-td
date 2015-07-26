#ifndef GRID_H
#define GRID_H

#include <math/Vec2.h>

#include <vector>

class Grid {
public:
    typedef std::vector<cocos2d::Vec2> Locations;

private:
    std::vector<std::vector<int>> mGrid;

public:
    Grid();

    Grid(cocos2d::Vec2);

    bool create(cocos2d::Vec2, int value = 0);

    Locations getNeighbours(cocos2d::Vec2) const;

    void fill(int);

    void clear() {
        mGrid.clear();
    }

    void setNode(cocos2d::Vec2 node, int value) {
        mGrid[node.x][node.y] = value;
    }

    int getNode(cocos2d::Vec2 node) const {
        return mGrid[node.x][node.y];
    }

    cocos2d::Vec2 getSize() const;

    bool isBounded(cocos2d::Vec2) const;

    void print();
};

#endif // GRID_H
