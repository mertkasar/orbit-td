#include <Entities/Towers/RLauncher.h>

#include <2d/CCSprite.h>

#include <Globals.h>
#include <Entities/Enemy.h>
#include <Entities/Missile.h>

USING_NS_CC;

bool RLauncher::init() {
    if (!Tower::init("textures/tower_base.png", "textures/r_launcher.png", 150.f, R_LAUNCHER_CD))
        return false;

    mGun->setScale(0.7f);

    return true;
}

void RLauncher::shoot(float pDelta) {
    auto projectile = Missile::create();
    projectile->setPosition(this->getPosition());
    projectile->setDamage(R_LAUNCHER_DMG);
    projectile->setTarget(mTarget);

    this->getParent()->addChild(projectile);

    CCLOG("Missile launched!");
}
