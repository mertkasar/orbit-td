#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <math/Vec2.h>

class Grid;

typedef std::vector<std::pair<cocos2d::Vec2, cocos2d::Vec2>> TraverseData;

namespace algorithm {
    cocos2d::Vec2 toCircularGrid(cocos2d::Vec2 pNode);

    TraverseData traverse(const Grid &, const cocos2d::Vec2 pStart, const cocos2d::Vec2 pGoal);

    bool isReached(const TraverseData &pTraversed, const cocos2d::Vec2 &pNode);

    std::vector<cocos2d::Vec2> calculatePath(const TraverseData &pTraversed, const cocos2d::Vec2 pStart,
                                             const cocos2d::Vec2 pGoal);
};


#endif //ALGORITHM_H
