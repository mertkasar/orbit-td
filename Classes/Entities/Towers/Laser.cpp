#include <Entities/Towers/Laser.h>

#include <2d/CCSprite.h>

#include <Entities/Creep.h>

USING_NS_CC;

bool Laser::init() {
    if (!Tower::init(models.at(TowerTypes::LASER)))
        return false;

    mTrace = DrawNode::create();
    this->addChild(mTrace);

    return true;
}

void Laser::shoot(float pDelta) {
    mTarget->deal(mDamage);
}
