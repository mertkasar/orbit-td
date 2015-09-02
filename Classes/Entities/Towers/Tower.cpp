#include <Entities/Towers/Tower.h>

#include <2d/CCSprite.h>
#include <physics/CCPhysicsBody.h>

#include <Globals.h>
#include <Entities/Enemy.h>

USING_NS_CC;

bool Tower::init(std::string pBaseTexturePath, std::string pGunTexturePath, float pRangeRadius, float pCooldown) {
    if (!Node::init())
        return false;

    mCooldown = pCooldown;

    mBase = Sprite::create(pBaseTexturePath);

    mGun = Sprite::create(pGunTexturePath);
    mGun->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);

    mBase->setScale(0.5f);

    mBody = PhysicsBody::createCircle(pRangeRadius);
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

void Tower::addTarget(Enemy *pTarget) {
    mRange.pushBack(pTarget);
}

void Tower::removeTarget(Enemy *pTarget) {
    auto found = mRange.find(pTarget);

    if (found != mRange.end())
        mRange.erase(found);
}

void Tower::update(float pDelta) {
    if (mTarget && mRange.find(mTarget) != mRange.end()) {
        if (!mTarget->isDead()) {
            adaptRotation();

            if (!this->isScheduled(CC_SCHEDULE_SELECTOR(Tower::shoot))) {
                shoot(0.f);
                this->schedule(CC_SCHEDULE_SELECTOR(Tower::shoot), mCooldown);
            }
        } else {
            mRange.eraseObject(mTarget);
            mTarget = nullptr;
            this->unschedule(CC_SCHEDULE_SELECTOR(Tower::shoot));
        }
    } else
        findTarget();
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
