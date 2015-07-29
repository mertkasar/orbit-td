#include <Entities/Tower.h>

#include <2d/CCSprite.h>
#include <physics/CCPhysicsBody.h>

#include <Globals.h>
#include <Entities/Enemy.h>

USING_NS_CC;

Tower::Tower() {
}

Tower::~Tower() {
}

bool Tower::init() {
    if (!Node::init())
        return false;

    mBase = Sprite::create("textures/tower_base.png");

    mGun = Sprite::create("textures/tower_gun.png");
    mGun->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);

    mBase->setScale(0.5f);

    mBody = PhysicsBody::createCircle(150.f);
    mBody->setCategoryBitmask(TOWER_RANGE_MASK);
    mBody->setContactTestBitmask(ENEMY_MASK);
    mBody->setCollisionBitmask(NULL_MASK);

    mTarget = nullptr;

    this->setPhysicsBody(mBody);

    this->setRotation(SPRITE_ANGLE);

    this->addChild(mBase);
    this->addChild(mGun);

    this->scheduleUpdate();

    return true;
}

void Tower::update(float pDelta) {
    if (mTarget) {
        if (isTargetValid()) {
            adaptRotation();
        } else
            mTarget = nullptr;
    } else
        findTarget();
}

void Tower::addTarget(Enemy *pTarget) {
    mRange.pushBack(pTarget);
}

void Tower::removeTarget(Enemy *pTarget) {
    auto found = mRange.find(pTarget);

    if (found != mRange.end())
        mRange.erase(found);
}

bool Tower::isTargetValid() {
    auto found = mRange.find(mTarget);

    return found != mRange.end();
}

void Tower::findTarget() {
    if (!mRange.empty())
        mTarget = mRange.at(0);
}

void Tower::adaptRotation() {
    Vec2 diff = mTarget->getPosition() - this->getPosition();
    diff.normalize();

    auto angle = CC_RADIANS_TO_DEGREES(diff.getAngle());

    mGun->setRotation(-1 * angle);
}
