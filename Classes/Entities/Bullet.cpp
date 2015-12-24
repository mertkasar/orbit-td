#include <Entities/Bullet.h>

#include <2d/CCSprite.h>
#include <2d/CCAction.h>
#include <physics/CCPhysicsBody.h>

#include <Utilities/SteeringDirector.h>
#include <Entities/Creep.h>
#include <2d/CCParticleSystem.h>
#include <2d/CCParticleExamples.h>
#include <Utilities/Shake.h>

USING_NS_CC;

#define BULLET_MAX_VEL 500.f

Bullet::Bullet() {
    CCLOG("Bullet created");
}

Bullet::~Bullet() {
    CCLOG("Bullet deleted");
}

bool Bullet::init() {
    if (!Node::init())
        return false;

    mSprite = Sprite::create("textures/bullet.png");
    mSprite->setScale(0.5f);
    this->addChild(mSprite);

    return true;
}

void Bullet::update(float pDelta) {
    if (mTarget != nullptr) {
        if (!mTarget->isDead())
            mTargetPosition = mTarget->getPosition();
        else
            mTarget = nullptr;
    }

    auto diff = mTargetPosition - this->getPosition();
    float reachRadius = 10 + mSprite->getContentSize().width / 2.f;

    if (diff.length() <= reachRadius) {
        if (mTarget != nullptr) {
            mTarget->deal(mDamage);
        }

        removeFromParent();
    } else {
        auto velocity = diff;
        velocity.normalize();
        velocity = velocity * BULLET_MAX_VEL;

        // Adapt rotation
        auto angle = CC_RADIANS_TO_DEGREES(velocity.getAngle());
        mSprite->setRotation(-angle);

        // Move the bullet
        this->setPosition(this->getPosition() + velocity * pDelta);
    }
}

void Bullet::ignite(cocos2d::Vec2 pPosition, const cocos2d::Color3B &pBaseColor, float pDamage, Creep *pTarget) {
    setDamage(pDamage);
    setTarget(pTarget);

    mSprite->setColor(pBaseColor);

    this->setPosition(pPosition);
    this->scheduleUpdate();
}
