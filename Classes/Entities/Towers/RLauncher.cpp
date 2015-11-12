#include <Entities/Towers/RLauncher.h>

#include <2d/CCSprite.h>

#include <Globals.h>
#include <Entities/Creep.h>
#include <Entities/Missile.h>

USING_NS_CC;

Pool<Missile> RLauncher::mMissilePool;

bool RLauncher::init() {
    if (!Tower::init("textures/r_launcher.png", 50, 150.f, 30.f, 1.f))
        return false;

    mGunSprite->setScale(0.7f);

    return true;
}

void RLauncher::shoot(float pDelta) {
    auto projectile = mMissilePool.fetch();
    projectile->ignite(this->getPosition(), mDamage, mTarget);

    this->getParent()->addChild(projectile);
}
