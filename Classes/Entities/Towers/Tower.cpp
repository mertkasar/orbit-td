#include <Entities/Towers/Tower.h>

#include <2d/CCSprite.h>
#include <physics/CCPhysicsBody.h>

#include <Globals.h>
#include <Entities/Creep.h>

USING_NS_CC;

bool Tower::init(std::string pBaseTexturePath, std::string pGunTexturePath, float pRangeRadius, float pCooldown,
                 unsigned int pCost) {
    if (!Node::init())
        return false;

    mCooldown = pCooldown;
    mCost = pCost;

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

void Tower::addTarget(Creep *pTarget) {
    mRange.pushBack(pTarget);
}

void Tower::removeTarget(Creep *pTarget) {
    auto found = mRange.find(pTarget);

    if (found != mRange.end())
        mRange.erase(found);
}

void Tower::update(float pDelta) {
    if (isTargetValid()) {
        adaptRotation();

        if (!this->isScheduled(CC_SCHEDULE_SELECTOR(Tower::shoot))) {
            shoot(0.f);
            this->schedule(CC_SCHEDULE_SELECTOR(Tower::shoot), mCooldown);
        }
    } else {
        if (mTarget != nullptr) {
            mRange.eraseObject(mTarget);
            mTarget = nullptr;
            this->unschedule(CC_SCHEDULE_SELECTOR(Tower::shoot));
        }

        findTarget();
    }
}

bool Tower::isTargetValid() {
    if (mTarget != nullptr) {
        auto found = mRange.find(mTarget);

        return found != mRange.end() && !mTarget->isDead();
    }

    return false;
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
