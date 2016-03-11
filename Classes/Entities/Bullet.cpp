#include "Bullet.h"

#include "EnemyShip.h"
#include "../Utilities/Shake.h"

#include <2d/CCSprite.h>
#include <2d/CCParticleSystem.h>
#include <2d/CCParticleExamples.h>
#include <physics/CCPhysicsBody.h>

#define BULLET_MAX_VEL 500.f

USING_NS_CC;

Bullet::Bullet() {
    CCLOG("Bullet created");
}

Bullet::~Bullet() {
    CCLOG("Bullet deleted");
}

bool Bullet::init() {
    if (!Node::init())
        return false;

    setCascadeOpacityEnabled(true);

    _sprite = Sprite::createWithSpriteFrameName("bullet.png");
    _sprite->setScale(0.5f);
    addChild(_sprite);

    return true;
}

void Bullet::update(float delta) {
    if (_target != nullptr) {
        if (!_target->isDead())
            _targetPosition = _target->getPosition();
        else
            _target = nullptr;
    }

    auto diff = _targetPosition - getPosition();
    float reachRadius = 10 + _sprite->getContentSize().width / 2.f;

    if (diff.length() <= reachRadius) {
        if (_target != nullptr) {
            _target->deal(_damage);
        }

        removeFromParent();
    } else {
        auto velocity = diff;
        velocity.normalize();
        velocity = velocity * BULLET_MAX_VEL;

        // Adapt rotation
        auto angle = CC_RADIANS_TO_DEGREES(velocity.getAngle());
        _sprite->setRotation(-angle);

        // Move the bullet
        setPosition(getPosition() + velocity * delta);
    }
}

void Bullet::restart(cocos2d::Vec2 position, const cocos2d::Color3B &baseColor, float damage, EnemyShip *target) {
    setDamage(damage);
    setTarget(target);

    _sprite->setColor(baseColor);

    setPosition(position);
    scheduleUpdate();
}
