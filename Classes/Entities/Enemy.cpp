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
    mBody->setMass(6.f);
    mBody->setVelocityLimit(ENEMY_MAX_VEL);
    this->setPhysicsBody(mBody);

    this->setScale(0.5f);
    this->setRotation(SPRITE_ANGLE);

    this->addChild(mSprite);

    this->scheduleUpdate();

    return true;
}

void Enemy::update(float pDelta) {
    SteeringDirector::getInstance()->seek(this, mTarget);

    // Adapt rotation
    auto angle = CC_RADIANS_TO_DEGREES(mBody->getVelocity().getAngle());
    mSprite->setRotation(-angle);
}
