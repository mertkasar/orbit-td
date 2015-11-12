#include <Entities/Missile.h>

#include <2d/CCSprite.h>
#include <2d/CCAction.h>
#include <physics/CCPhysicsBody.h>

#include <Utilities/SteeringDirector.h>
#include <Entities/Creep.h>

USING_NS_CC;

#define MISSILE_MAX_VEL 160.f
#define MISSILE_EXPIRE_TIME 4.f //as seconds

Missile::Missile() {
    CCLOG("Missile created");
}

Missile::~Missile() {
    CCLOG("Missile deleted");
}

bool Missile::init() {
    if (!Node::init())
        return false;

    mSprite = Sprite::create("textures/missile.png");

    mBody = PhysicsBody::createCircle(mSprite->getContentSize().width / 2.f);
    mBody->setCategoryBitmask(MISSILE_MASK);
    mBody->setContactTestBitmask(NULL_MASK);
    mBody->setCollisionBitmask(NULL_MASK);
    mBody->setMass(5.5f);
    mBody->setVelocityLimit(MISSILE_MAX_VEL);
    this->setPhysicsBody(mBody);

    this->addChild(mSprite);

    return true;
}

void Missile::update(float pDelta) {
    if (mTarget != nullptr) {
        if (!mTarget->isDead())
            mTargetPosition = mTarget->getPosition();
        else
            mTarget = nullptr;
    }

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
        removeFromParent();
}

void Missile::ignite(cocos2d::Vec2 pPosition, float pDamage, Creep *pTarget) {
    mDamage = 0.f;
    mDead = false;

    this->setPosition(pPosition);
    setDamage(pDamage);
    setTarget(pTarget);
    mBody->setVelocity(Vec2::ZERO);

    this->setScale(0.5f);
    this->setRotation(SPRITE_ANGLE);

    this->scheduleUpdate();
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(Missile::die), MISSILE_EXPIRE_TIME);
}

void Missile::die(float pDelta) {
    if (!mDead)
        mDead = true;
}
