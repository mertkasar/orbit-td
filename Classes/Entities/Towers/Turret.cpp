#include <Entities/Towers/Turret.h>

#include <2d/CCSprite.h>

#include <Globals.h>
#include <Entities/Creep.h>

bool Turret::init() {
    return Tower::init("textures/turret_gun.png", 10, 150.f, 3.f, 0.3f);
}

void Turret::shoot(float pDelta) {
    mTarget->deal(mDamage);
}
