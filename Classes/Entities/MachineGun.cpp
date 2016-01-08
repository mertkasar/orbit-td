#include "MachineGun.h"

#include "EnemyShip.h"
#include "../Scenes/GameplayLayer.h"

#include <2d/CCSprite.h>

USING_NS_CC;

MachineGun *MachineGun::create(const ValueMap &model) {
    MachineGun *obj = new(std::nothrow) MachineGun();

    if (obj && obj->init(model)) {
        obj->autorelease();
    } else {
        CC_SAFE_DELETE(obj);
    }

    return obj;
}

bool MachineGun::init(const ValueMap &model) {
    return Turret::init(model);
}

void MachineGun::shoot(float delta) {
    GameplayLayer *gameplayLayer = static_cast<GameplayLayer *>(getParent());
    const Vec2 &origin = _gunSprite->convertToWorldSpace(_muzzlePoint->getPosition());

    gameplayLayer->addBullet(origin, getBaseColor(), _damage, _target);
}
