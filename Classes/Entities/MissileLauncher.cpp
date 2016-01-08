#include "MissileLauncher.h"

#include "EnemyShip.h"
#include "../Scenes/GameplayLayer.h"

#include <2d/CCSprite.h>

USING_NS_CC;

MissileLauncher *MissileLauncher::create(const ValueMap &model) {
    MissileLauncher *obj = new(std::nothrow) MissileLauncher();

    if (obj && obj->init(model)) {
        obj->autorelease();
    } else {
        CC_SAFE_DELETE(obj);
    }

    return obj;
}

bool MissileLauncher::init(const ValueMap &model) {
    if (!Turret::init(model))
        return false;

    _gunSprite->setScale(0.7f);

    return true;
}

void MissileLauncher::shoot(float delta) {
    GameplayLayer *gameplayLayer = static_cast<GameplayLayer *>(getParent());
    const cocos2d::Vec2 &origin = _gunSprite->convertToWorldSpace(_muzzlePoint->getPosition());

    gameplayLayer->addMissile(origin, getBaseColor(), _damage, _target);
}
