#include <Entities/Towers/Turret.h>

#include <2d/CCSprite.h>

#include <Globals.h>
#include <Entities/Enemy.h>

bool Turret::init() {
    return Tower::init("textures/tower_base.png", "textures/turret_gun.png", 150.f, TURRET_CD, TURRET_COST);
}

void Turret::shoot(float pDelta) {
    mTarget->deal(TURRET_DMG);
}
