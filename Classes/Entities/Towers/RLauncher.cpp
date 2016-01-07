#include <Entities/Towers/RLauncher.h>

#include <2d/CCSprite.h>

#include <Entities/Creep.h>
#include <Scenes/GameplayLayer.h>

USING_NS_CC;

bool RLauncher::init() {
    if (!Tower::init(towerModels.at(ModelID::R_LAUNCHER)))
        return false;

    _gunSprite->setScale(0.7f);

    return true;
}

void RLauncher::shoot(float delta) {
    GameplayLayer *gameplayLayer = static_cast<GameplayLayer *>(getParent());
    const cocos2d::Vec2 &origin = _gunSprite->convertToWorldSpace(_muzzlePoint->getPosition());

    gameplayLayer->addMissile(origin, getBaseColor(), _damage, _target);
}
