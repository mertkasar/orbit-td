#include <Entities/Enemy.h>

#include <Globals.h>
#include <2d/CCSprite.h>
#include <physics/CCPhysicsBody.h>
#include <Utilities/SteeringDirector.h>
#include <2d/CCAction.h>

USING_NS_CC;

Enemy::Enemy() {
}

Enemy::~Enemy() {
}

bool Enemy::init() {
    if (!Node::init())
        return false;

    mHitPoints = ENEMY_HP;

    mSprite = Sprite::create("textures/enemy.png");

    mBody = PhysicsBody::createCircle(mSprite->getContentSize().width / 2.f);
    mBody->setCategoryBitmask(ENEMY_MASK);
    mBody->setContactTestBitmask(TOWER_RANGE_MASK);
    mBody->setCollisionBitmask(NULL_MASK);
    mBody->setMass(5.5f);
    mBody->setVelocityLimit(ENEMY_MAX_VEL);
    this->setPhysicsBody(mBody);

    this->setScale(0.5f);
    this->setRotation(SPRITE_ANGLE);

    this->addChild(mSprite);

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

    if (mHitPoints <= 0.f)
        dead = true;

    if (dead)
        removeFromParentAndCleanup(true);
}
