#include <Entities/Tower.h>

#include <Globals.h>
#include <2d/CCSprite.h>
#include <physics/CCPhysicsBody.h>

using namespace cocos2d;

Tower::Tower() {
    CCLOG("Tower created.");
}

Tower::~Tower() {
    CCLOG("Tower deleted.");
}

bool Tower::init() {
    if (!Node::init())
        return false;

    mBase = Sprite::create("textures/tower_base.png");

    mGun = Sprite::create("textures/tower_gun.png");
    mGun->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);

    mBase->setScale(0.5f);

    mBody = PhysicsBody::createCircle(100.f);
    mBody->setCategoryBitmask(TOWER_RANGE_MASK);
    mBody->setContactTestBitmask(ENEMY_MASK);
    mBody->setCollisionBitmask(NULL_MASK);
    this->setPhysicsBody(mBody);

    this->addChild(mBase);
    this->addChild(mGun);

    return true;
}
