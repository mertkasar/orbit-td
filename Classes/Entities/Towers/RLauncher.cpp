#include <Entities/Towers/RLauncher.h>

#include <2d/CCSprite.h>

#include <Entities/Creep.h>
#include <Entities/Missile.h>

USING_NS_CC;

Pool<Missile> RLauncher::mMissilePool;

bool RLauncher::init() {
    if (!Tower::init(models.at(TowerTypes::R_LAUNCHER)))
        return false;

    mGunSprite->setScale(0.7f);

    return true;
}

void RLauncher::shoot(float pDelta) {
    auto projectile = mMissilePool.fetch();
    projectile->ignite(this->getPosition(), getBaseColor(), mDamage, mTarget);

    this->getParent()->addChild(projectile);
}
