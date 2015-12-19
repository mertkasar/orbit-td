#include <Entities/Towers/Laser.h>

#include <2d/CCSprite.h>

#include <Entities/Creep.h>
#include <Entities/Beam.h>

USING_NS_CC;

bool Laser::init() {
    if (!Tower::init(models.at(TowerTypes::LASER)))
        return false;

    mBeam = Beam::create();
    mBeam->setColor(getBaseColor());

    auto size = mGunSprite->getContentSize();
    mBeam->setPosition(size.width, size.height / 2.f);
    mBeam->setVisible(false);
    mGunSprite->addChild(mBeam);

    return true;
}

void Laser::update(float pDelta) {
    if (isTargetValid()) {
        if (!mBeam->isVisible()) {
            mBeam->setVisible(true);
        }
    } else {
        if (mBeam->isVisible()) {
            mBeam->setVisible(false);
        }
    }

    Tower::update(pDelta);
}

void Laser::upgrade(cocos2d::Color3B &pColor) {
    Tower::upgrade(pColor);

    mBeam->setColor(pColor);
}

void Laser::shoot(float pDelta) {
    mTarget->deal(mDamage);
    mBeam->update(mTarget->getPosition());
}
