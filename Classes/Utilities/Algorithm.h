#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "../Globals.h"

#include <math/Vec2.h>

#include <map>

namespace algorithm {
    cocos2d::Vec2 toCircularGrid(cocos2d::Vec2 node);

    cocos2d::Vec2 seek(cocos2d::Node *seeker, cocos2d::Vec2 seeking);
};


#endif //ALGORITHM_H
