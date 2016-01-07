#include "LaserGun.h"

#include <2d/CCSprite.h>

#include "EnemyShip.h"
#include "LaserBeam.h"

USING_NS_CC;

bool LaserGun::init() {
    if (!Turret::init(_models.at(ModelID::LASER_GUN)))
        return false;

    _beam = LaserBeam::create();
    _beam->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _beam->setColor(getBaseColor());
    _beam->setVisible(false);
    _muzzlePoint->addChild(_beam);

    return true;
}

void LaserGun::update(float delta) {
    if (isTargetValid()) {
        if (!_beam->isVisible()) {
            _beam->setVisible(true);
        }
    } else {
        if (_beam->isVisible()) {
            _beam->setVisible(false);
        }
    }

    Turret::update(delta);
}

void LaserGun::upgrade(cocos2d::Color3B &color) {
    Turret::upgrade(color);

    _beam->setColor(color);
}

void LaserGun::shoot(float pDelta) {
    _target->deal(_damage);
    _beam->update(_target->getPosition());
}
