#include <Entities/Towers/Laser.h>

#include <2d/CCSprite.h>
#include <2d/CCDrawNode.h>

#include <Globals.h>
#include <Entities/Creep.h>

USING_NS_CC;

bool Laser::init() {
    if (!Tower::init("textures/laser_gun.png", 30, 150.f, 0.3f, 0.f))
        return false;

    mTrace = DrawNode::create();
    this->addChild(mTrace);

    return true;
}

void Laser::shoot(float pDelta) {
    mTarget->deal(mDamage);
}
