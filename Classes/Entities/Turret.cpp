#include <Entities/Turret.h>

#include <2d/CCSprite.h>

#include <Globals.h>
#include <Entities/Enemy.h>

bool Turret::init() {
    return Tower::init("textures/tower_base.png", "textures/tower_gun.png", 150.f, TURRET_CD);
}

void Turret::shoot(float pDelta) {
    mTarget->deal(TURRET_DMG);
}
