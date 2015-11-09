#include <Entities/Towers/RLauncher.h>

#include <2d/CCSprite.h>

#include <Globals.h>
#include <Entities/Creep.h>
#include <Entities/Missile.h>

USING_NS_CC;

Pool<Missile> RLauncher::mMissilePool;

bool RLauncher::init() {
    if (!Tower::init("textures/r_launcher.png", 150.f, R_LAUNCHER_CD, R_LAUNCHER_COST))
        return false;

    mGun->setScale(0.7f);

    return true;
}

void RLauncher::shoot(float pDelta) {
    auto projectile = mMissilePool.fetch();
    auto damage = R_LAUNCHER_DMG + mLevel * 10 * 0.1;
    projectile->ignite(this->getPosition(), (float) damage, mTarget);

    this->getParent()->addChild(projectile);
}
