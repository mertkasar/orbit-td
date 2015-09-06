#include <Entities/Creeps/Creep.h>

#include <Globals.h>
#include <2d/CCSprite.h>
#include <2d/CCAction.h>
#include <2d/CCDrawNode.h>
#include <physics/CCPhysicsBody.h>
#include <Utilities/SteeringDirector.h>

USING_NS_CC;

Creep::Creep() {
    CCLOG("Creep created");
}

Creep::~Creep() {
    CCLOG("Creep deleted");
}

bool Creep::init() {
    if (!Node::init())
        return false;

    mMaxHP = ENEMY_HP;
    mReward = ENEMY_REWARD;

    mKilled = false;
    mReachedEnd = false;

    mSprite = Sprite::create("textures/creeps/creep.png");

    mBody = PhysicsBody::createCircle(mSprite->getContentSize().width / 2.f);
    mBody->setCategoryBitmask(ENEMY_MASK);
    mBody->setContactTestBitmask(TOWER_RANGE_MASK);
    mBody->setCollisionBitmask(NULL_MASK);
    mBody->setMass(5.5f);
    mBody->setVelocityLimit(ENEMY_MAX_VEL);
    this->setPhysicsBody(mBody);

    mHPBar = DrawNode::create();
    mHPBar->setPosition(Vec2(-40.f, -70.f));

    this->addChild(mSprite);
    this->addChild(mHPBar);

    return true;
}

void Creep::update(float pDelta) {
    WayPoint target = mPath.getNextWaypoint();
    float reachRadius = target.reachRadius + mSprite->getContentSize().width / 2.f;
    if (this->getPosition().distance(target.location) <= reachRadius) {
        if (mPath.eop())
            mReachedEnd = true;
        else {
            mPath.forward();
            target = mPath.getNextWaypoint();
        }
    }

    SteeringDirector::getInstance()->seek(this, target.location);

    // Adapt rotation
    auto angle = CC_RADIANS_TO_DEGREES(mBody->getVelocity().getAngle());
    mSprite->setRotation(-angle);

    if (mCurrentHP <= 0.f)
        mKilled = true;
}

void Creep::ignite(cocos2d::Vec2 pPosition, const Path &pPath) {
    mCurrentHP = mMaxHP;

    mKilled = false;
    mReachedEnd = false;

    this->setPosition(pPosition);
    this->setScale(0.5f);
    this->setRotation(SPRITE_ANGLE);
    mBody->setVelocity(Vec2::ZERO);

    mPath.clone(pPath);

    updateHPBar();

    this->scheduleUpdate();
}

void Creep::deal(float pDamage) {
    mCurrentHP = mCurrentHP - pDamage;
    updateHPBar();
}

void Creep::updateHPBar() {
    mHPBar->clear();

    if (mCurrentHP < 0.f)
        mCurrentHP = 0.f;

    mHPBar->drawSolidRect(Vec2::ZERO, Vec2(HBAR_HEIGHT, HBAR_WIDTH), Color4F::RED);
    mHPBar->drawSolidRect(Vec2::ZERO, Vec2(mCurrentHP * HBAR_HEIGHT / mMaxHP, HBAR_WIDTH), Color4F::GREEN);
}
