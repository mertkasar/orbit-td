#include <Entities/Towers/Laser.h>

#include <2d/CCSprite.h>

#include <Entities/Creep.h>
#include <Entities/Beam.h>

USING_NS_CC;

bool Laser::init() {
    if (!Tower::init(_models.at(ModelID::LASER)))
        return false;

    _beam = Beam::create();
    _beam->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _beam->setColor(getBaseColor());
    _beam->setVisible(false);
    _muzzlePoint->addChild(_beam);

    return true;
}

void Laser::update(float delta) {
    if (isTargetValid()) {
        if (!_beam->isVisible()) {
            _beam->setVisible(true);
        }
    } else {
        if (_beam->isVisible()) {
            _beam->setVisible(false);
        }
    }

    Tower::update(delta);
}

void Laser::upgrade(cocos2d::Color3B &color) {
    Tower::upgrade(color);

    _beam->setColor(color);
}

void Laser::shoot(float pDelta) {
    _target->deal(_damage);
    _beam->update(_target->getPosition());
}
