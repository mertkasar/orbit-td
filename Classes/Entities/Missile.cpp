#include <Entities/Missile.h>

#include <2d/CCSprite.h>
#include <2d/CCAction.h>
#include <physics/CCPhysicsBody.h>

#include <Utilities/SteeringDirector.h>
#include <Entities/Creep.h>
#include <2d/CCParticleSystem.h>
#include <2d/CCParticleExamples.h>
#include <Utilities/Shake.h>

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

    mSprite = Sprite::createWithSpriteFrameName("missile.png");

    mBody = PhysicsBody::createCircle(mSprite->getContentSize().width / 2.f);
    mBody->setCategoryBitmask(MISSILE_MASK);
    mBody->setContactTestBitmask(NULL_MASK);
    mBody->setCollisionBitmask(NULL_MASK);
    mBody->setMass(5.5f);
    mBody->setVelocityLimit(MISSILE_MAX_VEL);
    this->setPhysicsBody(mBody);

    mFireEmitter = ParticleSystemQuad::create("missile_fire.plist");
    mFireEmitter->setPosition(this->getPosition());

    this->addChild(mSprite);

    return true;
}

void Missile::update(float pDelta) {
    mFireEmitter->setPosition(this->getPosition());

    if (mTarget != nullptr) {
        if (!mTarget->isDead())
            mTargetPosition = mTarget->getPosition();
        else
            mTarget = nullptr;
    }

    float reachRadius = 10 + mSprite->getContentSize().width / 2.f;
    if (this->getPosition().distance(mTargetPosition) <= reachRadius) {
        die(pDelta);

        if (mTarget != nullptr) {
            mTarget->deal(mDamage);
            mTarget->runAction(Shake::actionWithDuration(0.4f, 1.3f));
        }
    }

    SteeringDirector::getInstance()->seek(this, mTargetPosition);

    // Adapt rotation
    auto angle = CC_RADIANS_TO_DEGREES(mBody->getVelocity().getAngle());
    mSprite->setRotation(-angle);

    if (isDead()) {
        mFireEmitter->stopSystem();
        removeFromParent();
    }
}

void Missile::restart(cocos2d::Vec2 pPosition, const cocos2d::Color3B &pBaseColor, float pDamage, Creep *pTarget) {
    mDead = false;

    this->setPosition(pPosition);
    setDamage(pDamage);
    setTarget(pTarget);
    mBody->setVelocity(Vec2::ZERO);

    this->setScale(0.5f);

    mFireEmitter->setStartColor(Color4F(pBaseColor));

    //TODO: This doesn't work as I expected, I need a resume operation not a reset!
    mFireEmitter->resetSystem();

    this->scheduleUpdate();
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(Missile::die), MISSILE_EXPIRE_TIME);
}

void Missile::die(float pDelta) {
    if (!mDead)
        mDead = true;
}
