#include <Entities/Missile.h>

#include <Globals.h>
#include <2d/CCSprite.h>
#include <2d/CCAction.h>
#include <physics/CCPhysicsBody.h>

#include <Utilities/SteeringDirector.h>
#include <Entities/Enemy.h>

USING_NS_CC;

Missile::Missile() {
}

Missile::~Missile() {
}

bool Missile::init() {
    if (!Node::init())
        return false;

    mDamage = 0.f;

    mDead = false;

    mSprite = Sprite::create("textures/missile.png");

    mBody = PhysicsBody::createCircle(mSprite->getContentSize().width / 2.f);
    mBody->setCategoryBitmask(MISSILE_MASK);
    mBody->setContactTestBitmask(NULL_MASK);
    mBody->setCollisionBitmask(NULL_MASK);
    mBody->setMass(5.5f);
    mBody->setVelocityLimit(MISSILE_MAX_VEL);
    this->setPhysicsBody(mBody);

    this->setScale(0.5f);
    this->setRotation(SPRITE_ANGLE);

    this->addChild(mSprite);

    this->scheduleUpdate();
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(Missile::die), MISSILE_EXPIRE_TIME);

    return true;
}

void Missile::update(float pDelta) {
    if (mTarget != nullptr)
        mTargetPosition = mTarget->getPosition();

    float reachRadius = 10 + mSprite->getContentSize().width / 2.f;
    if (this->getPosition().distance(mTargetPosition) <= reachRadius) {
        die(pDelta);

        if (mTarget != nullptr)
            mTarget->deal(mDamage);
    }

    SteeringDirector::getInstance()->seek(this, mTargetPosition);

    // Adapt rotation
    auto angle = CC_RADIANS_TO_DEGREES(mBody->getVelocity().getAngle());
    mSprite->setRotation(-angle);


    if (isDead())
        removeFromParentAndCleanup(true);
}

void Missile::die(float pDelta) {
    if (!mDead)
        mDead = true;
}
