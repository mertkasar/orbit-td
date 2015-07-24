#include <Entities/Enemy.h>

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

    this->setScale(0.5f);

    this->addChild(mSprite);

    return true;
}
