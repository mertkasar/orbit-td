#include <Entities/Towers/Laser.h>

#include <2d/CCSprite.h>
#include <2d/CCDrawNode.h>

#include <Globals.h>
#include <Entities/Enemy.h>

USING_NS_CC;

bool Laser::init() {
    if (!Tower::init("textures/tower_base.png", "textures/laser_gun.png", 150.f, LASER_CD))
        return false;

    mTrace = DrawNode::create();
    this->addChild(mTrace);

    return true;
}

void Laser::shoot(float pDelta) {
    mTarget->deal(LASER_DMG);
}