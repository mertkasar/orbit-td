#include <Entities/Towers/Turret.h>

#include <2d/CCSprite.h>

#include <Entities/Creep.h>
#include <Scenes/GameplayLayer.h>

bool Turret::init() {
    return Tower::init(models.at(TowerTypes::TURRET));
}

void Turret::shoot(float pDelta) {
    static_cast<GameplayLayer *>(this->getParent())->addBullet(this->getPosition(), getBaseColor(), mDamage, mTarget);
}
