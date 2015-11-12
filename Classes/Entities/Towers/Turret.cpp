#include <Entities/Towers/Turret.h>

#include <2d/CCSprite.h>

#include <Entities/Creep.h>

bool Turret::init() {
    return Tower::init(models.at(TowerTypes::TURRET));
}

void Turret::shoot(float pDelta) {
    mTarget->deal(mDamage);
}
