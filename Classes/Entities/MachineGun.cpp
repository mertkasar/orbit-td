#include "MachineGun.h"

#include <2d/CCSprite.h>

#include "EnemyShip.h"
#include <Scenes/GameplayLayer.h>

bool MachineGun::init() {
    return Turret::init(_models.at(ModelID::MACHINE_GUN));
}

void MachineGun::shoot(float delta) {
    GameplayLayer *gameplayLayer = static_cast<GameplayLayer *>(getParent());
    const cocos2d::Vec2 &origin = _gunSprite->convertToWorldSpace(_muzzlePoint->getPosition());

    gameplayLayer->addBullet(origin, getBaseColor(), _damage, _target);
}
