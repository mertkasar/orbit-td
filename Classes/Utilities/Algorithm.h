#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <map>

#include <math/Vec2.h>

#include <Globals.h>

typedef std::map<cocos2d::Vec2, cocos2d::Vec2> TraverseData;

class Grid;

namespace algorithm {
    cocos2d::Vec2 toCircularGrid(cocos2d::Vec2 pNode);

    TraverseData traverse(const Grid &, const cocos2d::Vec2 pStart, const cocos2d::Vec2 pGoal);
};


#endif //ALGORITHM_H
