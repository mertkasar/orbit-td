#include <Entities/Enemy.h>

#include <Globals.h>
#include <2d/CCSprite.h>
#include <2d/CCAction.h>
#include <2d/CCDrawNode.h>
#include <physics/CCPhysicsBody.h>
#include <Utilities/SteeringDirector.h>

USING_NS_CC;

Enemy::Enemy() {
}

Enemy::~Enemy() {
}

bool Enemy::init() {
    if (!Node::init())
        return false;

    mMaxHP = ENEMY_HP;
    mCurrentHP = mMaxHP;

    mSprite = Sprite::create("textures/enemy.png");

    mBody = PhysicsBody::createCircle(mSprite->getContentSize().width / 2.f);
    mBody->setCategoryBitmask(ENEMY_MASK);
    mBody->setContactTestBitmask(TOWER_RANGE_MASK);
    mBody->setCollisionBitmask(NULL_MASK);
    mBody->setMass(5.5f);
    mBody->setVelocityLimit(ENEMY_MAX_VEL);
    this->setPhysicsBody(mBody);

    mHPBar = DrawNode::create();
    mHPBar->setPosition(Vec2(-40.f, -70.f));
    updateHPBar();

    this->setScale(0.5f);
    this->setRotation(SPRITE_ANGLE);

    this->addChild(mSprite);
    this->addChild(mHPBar);

    this->scheduleUpdate();

    return true;
}

void Enemy::update(float pDelta) {
    bool dead = false;

    WayPoint target = mPath.getNextWaypoint();
    float reachRadius = target.reachRadius + mSprite->getContentSize().width / 2.f;
    if (this->getPosition().distance(target.location) <= reachRadius) {
        if (mPath.eop())
            dead = true;
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
        dead = true;

    if (dead)
        removeFromParentAndCleanup(true);
}

void Enemy::deal(float pDamage) {
    mCurrentHP = mCurrentHP - pDamage;
    updateHPBar();
}

void Enemy::updateHPBar() {
    mHPBar->clear();

    mHPBar->drawSolidRect(Vec2::ZERO, Vec2(HBAR_HEIGHT, HBAR_WIDTH), Color4F::RED);
    mHPBar->drawSolidRect(Vec2::ZERO, Vec2(mCurrentHP * HBAR_HEIGHT / mMaxHP, HBAR_WIDTH), Color4F::GREEN);
}
