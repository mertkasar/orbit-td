#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <math/Vec2.h>

class Grid;

typedef std::vector<std::pair<cocos2d::Vec2, cocos2d::Vec2>> TraverseData;

namespace algorithm {
    cocos2d::Vec2 toCircularGrid(cocos2d::Vec2 pNode);

    TraverseData traverse(const Grid &, cocos2d::Vec2, cocos2d::Vec2);
};


#endif //ALGORITHM_H
