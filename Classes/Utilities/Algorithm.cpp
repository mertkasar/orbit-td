#include <Utilities/Algorithm.h>

#include <queue>

#include <Utilities/Grid.h>

USING_NS_CC;

#define GRID_RADIUS 600.f
#define GRID_START_ANGLE 20.f
#define GRID_SHIFT_ANGLE 10.f

cocos2d::Vec2 algorithm::toCircularGrid(Vec2 node) {
    float angle = GRID_START_ANGLE - node.x * GRID_SHIFT_ANGLE;

    Vec2 position = Vec2((float) (GRID_RADIUS * cos(CC_DEGREES_TO_RADIANS(angle))),
                         (float) (GRID_RADIUS * sin(CC_DEGREES_TO_RADIANS(angle))));

    Vec2 shift = node.y * Vec2(100.f, 0);

    return position + shift + Vec2(-400.f, 360.f);
}

