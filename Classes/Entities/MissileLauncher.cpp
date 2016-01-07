#include "MissileLauncher.h"

#include <2d/CCSprite.h>

#include "EnemyShip.h"
#include <Scenes/GameplayLayer.h>

USING_NS_CC;

bool MissileLauncher::init() {
    if (!Turret::init(_models.at(ModelID::MISSILE_LAUNCHER)))
        return false;

    _gunSprite->setScale(0.7f);

    return true;
}

void MissileLauncher::shoot(float delta) {
    GameplayLayer *gameplayLayer = static_cast<GameplayLayer *>(getParent());
    const cocos2d::Vec2 &origin = _gunSprite->convertToWorldSpace(_muzzlePoint->getPosition());

    gameplayLayer->addMissile(origin, getBaseColor(), _damage, _target);
}
