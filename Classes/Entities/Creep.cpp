#include <Entities/Creep.h>

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

    mKilled = false;
    mReachedEnd = false;

    mSprite = Sprite::create();
    mSprite->setContentSize(Size(100, 90));

    mBody = PhysicsBody::createCircle(mSprite->getContentSize().width / 2.f);
    mBody->setCategoryBitmask(ENEMY_MASK);
    mBody->setContactTestBitmask(TOWER_RANGE_MASK);
    mBody->setCollisionBitmask(NULL_MASK);
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

void Creep::ignite(CreepTypes pType, cocos2d::Vec2 pPosition, const Path &pPath) {
    mKilled = false;
    mReachedEnd = false;

    reShape(pType);

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

void Creep::reShape(CreepTypes pType) {
    switch (pType) {
        case RAPTOR:
            mSprite->setTexture("textures/creeps/raptor.png");
            mMaxHP = 100.f;
            mReward = 10;
            mBody->setMass(5.5f);
            mBody->setVelocityLimit(80.f);
            break;

        case SPEEDY:
            mSprite->setTexture("textures/creeps/speedy.png");
            mMaxHP = 50.f;
            mReward = 5;
            mBody->setMass(3.5f);
            mBody->setVelocityLimit(130.f);
            break;

        case PULSAR:
            mSprite->setTexture("textures/creeps/pulsar.png");
            mMaxHP = 130.f;
            mReward = 25;
            mBody->setMass(5.5f);
            mBody->setVelocityLimit(80.f);
            break;

        case PANZER:
            mSprite->setTexture("textures/creeps/panzer.png");
            mMaxHP = 300.f;
            mReward = 20;
            mBody->setMass(6.5f);
            mBody->setVelocityLimit(40.f);
            break;

        default:
            break;
    }

    mCurrentHP = mMaxHP;
}
