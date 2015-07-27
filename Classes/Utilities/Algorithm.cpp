#include <Utilities/Algorithm.h>

#include <Globals.h>
#include <base/ccMacros.h>

USING_NS_CC;

cocos2d::Vec2 algorithm::toCircularGrid(Vec2 pNode) {
    float angle = GRID_START_ANGLE - pNode.x * GRID_SHIFT_ANGLE;

    Vec2 position = Vec2((float) (GRID_RADIUS * cos(CC_DEGREES_TO_RADIANS(angle))),
                         (float) (GRID_RADIUS * sin(CC_DEGREES_TO_RADIANS(angle))));

    Vec2 shift = pNode.y * Vec2(100.f, 0);

    return position + shift;
}
