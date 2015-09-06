#include <Entities/Towers/Laser.h>

#include <2d/CCSprite.h>
#include <2d/CCDrawNode.h>

#include <Globals.h>
#include <Entities/Creeps/Creep.h>

USING_NS_CC;

bool Laser::init() {
    if (!Tower::init("textures/tower_base.png", "textures/laser_gun.png", 150.f, LASER_CD, LASER_COST))
        return false;

    mTrace = DrawNode::create();
    this->addChild(mTrace);

    return true;
}

void Laser::shoot(float pDelta) {
    mTarget->deal(LASER_DMG);
}
