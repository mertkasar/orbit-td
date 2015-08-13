#include <Entities/Towers/RLauncher.h>

#include <2d/CCSprite.h>

#include <Globals.h>
#include <Entities/Enemy.h>

bool RLauncher::init() {
    if (!Tower::init("textures/tower_base.png", "textures/r_launcher.png", 150.f, R_LAUNCHER_CD))
        return false;

    mGun->setScale(0.7f);

    return true;
}

void RLauncher::shoot(float pDelta) {
    mTarget->deal(R_LAUNCHER_DMG);
}
