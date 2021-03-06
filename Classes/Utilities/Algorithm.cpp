#include "Algorithm.h"

#include "2d/CCNode.h"
#include "physics/CCPhysicsBody.h"

#define GRID_RADIUS 600.f
#define GRID_START_ANGLE 20.f
#define GRID_SHIFT_ANGLE 10.f

USING_NS_CC;

cocos2d::Vec2 algorithm::toCircularGrid(Vec2 node) {
    float angle = GRID_START_ANGLE - node.x * GRID_SHIFT_ANGLE;

    Vec2 position = Vec2((float) (GRID_RADIUS * cos(CC_DEGREES_TO_RADIANS(angle))),
                         (float) (GRID_RADIUS * sin(CC_DEGREES_TO_RADIANS(angle))));

    Vec2 shift = node.y * Vec2(100.f, 0);

    return position + shift + Vec2(-400.f, 360.f);
}

Vec2 algorithm::seek(Node *seeker, Vec2 seeking) {
    auto seekerBody = seeker->getPhysicsBody();

    CC_ASSERT(seekerBody != nullptr);

    Vec2 direction = seeking - seeker->getPosition();
    direction.normalize();

    Vec2 steeringForce = direction * seekerBody->getVelocityLimit();
    steeringForce = steeringForce / seekerBody->getMass();

    return steeringForce;
}

