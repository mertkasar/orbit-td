#include <Entities/Enemy.h>

#include <Globals.h>
#include <2d/CCSprite.h>
#include <physics/CCPhysicsBody.h>
#include <Utilities/SteeringDirector.h>
#include <2d/CCAction.h>

USING_NS_CC;

Enemy::Enemy() {
    CCLOG("Enemy created.");
}

Enemy::~Enemy() {
    CCLOG("Enemy deleted.");
}

bool Enemy::init() {
    if (!Node::init())
        return false;

    mSprite = Sprite::create("textures/enemy.png");

    mBody = PhysicsBody::createCircle(mSprite->getContentSize().width / 2.f);
    mBody->setCategoryBitmask(ENEMY_MASK);
    mBody->setContactTestBitmask(TOWER_RANGE_MASK);
    mBody->setCollisionBitmask(NULL_MASK);
    mBody->setMass(4.f);
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

    Vec2 position = this->getPosition();
    Vec2 target = mPath.getCurrentNode();

    if (position.distance(target) <= mPath.getDensity()) {
        if (mPath.eop())
            dead = true;
        else {
            mPath.forward();
            target = mPath.getCurrentNode();
        }
    }

    SteeringDirector::getInstance()->seek(this, target);

    // Adapt rotation
    auto angle = CC_RADIANS_TO_DEGREES(mBody->getVelocity().getAngle());
    mSprite->setRotation(-angle);

    if (dead)
        removeFromParentAndCleanup(true);
}

void Enemy::constructPath(const Path &pPath) {
    for (auto node : pPath.getWayPointList())
        mPath.addNode(node);

}
