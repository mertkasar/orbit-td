#include <Entities/Towers/Laser.h>

#include <2d/CCSprite.h>
#include <2d/CCDrawNode.h>

#include <Globals.h>
#include <Entities/Creep.h>

USING_NS_CC;

bool Laser::init() {
    if (!Tower::init("textures/laser_gun.png", LASER_CD, LASER_COST))
        return false;

    mTrace = DrawNode::create();
    this->addChild(mTrace);

    return true;
}

void Laser::shoot(float pDelta) {
    mTarget->deal((float) (LASER_DMG + mLevel * 10 * 0.1));
}
