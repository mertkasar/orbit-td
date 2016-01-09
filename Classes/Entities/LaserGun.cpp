#include "LaserGun.h"

#include "EnemyShip.h"
#include "LaserBeam.h"

#include <2d/CCSprite.h>

USING_NS_CC;

LaserGun *LaserGun::create(const ValueMap &model) {
    LaserGun *obj = new(std::nothrow) LaserGun();

    if (obj && obj->init(model)) {
        obj->autorelease();
    } else {
        CC_SAFE_DELETE(obj);
    }

    return obj;
}

bool LaserGun::init(const ValueMap &model) {
    if (!Turret::init(model))
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

void LaserGun::upgrade() {
    Turret::upgrade();

    _beam->setColor(getBaseColor());
}

void LaserGun::shoot(float pDelta) {
    _target->deal(_damage);
    _beam->update(_target->getPosition());
}
