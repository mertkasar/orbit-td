#include <Entities/Towers/Turret.h>

#include <2d/CCSprite.h>

#include <Globals.h>
#include <Entities/Creep.h>

bool Turret::init() {
    return Tower::init("textures/turret_gun.png", TURRET_CD, TURRET_COST);
}

void Turret::shoot(float pDelta) {
    mTarget->deal((float) (TURRET_DMG + mLevel * 10 * 0.1));
}
