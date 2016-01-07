#include <Entities/Towers/Turret.h>

#include <2d/CCSprite.h>

#include <Entities/Creep.h>
#include <Scenes/GameplayLayer.h>

bool Turret::init() {
    return Tower::init(_models.at(ModelID::TURRET));
}

void Turret::shoot(float delta) {
    GameplayLayer *gameplayLayer = static_cast<GameplayLayer *>(getParent());
    const cocos2d::Vec2 &origin = _gunSprite->convertToWorldSpace(_muzzlePoint->getPosition());

    gameplayLayer->addBullet(origin, getBaseColor(), _damage, _target);
}
