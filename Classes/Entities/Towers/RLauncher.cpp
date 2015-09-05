#include <Entities/Towers/RLauncher.h>

#include <2d/CCSprite.h>

#include <Globals.h>
#include <Entities/Enemy.h>
#include <Entities/Missile.h>

USING_NS_CC;

Pool<Missile> RLauncher::mMissilePool;

bool RLauncher::init() {
    if (!Tower::init("textures/tower_base.png", "textures/r_launcher.png", 150.f, R_LAUNCHER_CD, R_LAUNCHER_COST))
        return false;

    mGun->setScale(0.7f);

    return true;
}

void RLauncher::shoot(float pDelta) {
    auto projectile = mMissilePool.fetch();
    projectile->ignite(this->getPosition(), R_LAUNCHER_DMG, mTarget);

    this->getParent()->addChild(projectile);
}
