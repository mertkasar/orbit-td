#include <Entities/Enemy.h>

#include <Globals.h>
#include <2d/CCSprite.h>
#include <physics/CCPhysicsBody.h>

using namespace cocos2d;

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
    this->setPhysicsBody(mBody);

    this->setScale(0.5f);
    this->setRotation(SPRITE_ANGLE);

    this->addChild(mSprite);

    return true;
}
