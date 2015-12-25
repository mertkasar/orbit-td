#include <Entities/Towers/Turret.h>

#include <2d/CCSprite.h>

#include <Entities/Creep.h>
#include <Scenes/GameplayLayer.h>

bool Turret::init() {
    return Tower::init(models.at(TowerTypes::TURRET));
}

void Turret::shoot(float pDelta) {
    GameplayLayer *gameplayLayer = static_cast<GameplayLayer *>(this->getParent());
    const cocos2d::Vec2 &origin = mGunSprite->convertToWorldSpace(mMuzzlePoint->getPosition());

    gameplayLayer->addBullet(origin, getBaseColor(), mDamage, mTarget);
}
