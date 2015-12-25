#include <Entities/Towers/RLauncher.h>

#include <2d/CCSprite.h>

#include <Entities/Creep.h>
#include <Scenes/GameplayLayer.h>

USING_NS_CC;

bool RLauncher::init() {
    if (!Tower::init(models.at(TowerTypes::R_LAUNCHER)))
        return false;

    mGunSprite->setScale(0.7f);

    return true;
}

void RLauncher::shoot(float pDelta) {
    GameplayLayer *gameplayLayer = static_cast<GameplayLayer *>(this->getParent());
    const cocos2d::Vec2 &origin = mGunSprite->convertToWorldSpace(mMuzzlePoint->getPosition());

    gameplayLayer->addMissile(origin, getBaseColor(), mDamage, mTarget);
}
