#include <Entities/Towers/Tower.h>

#include <2d/CCSprite.h>
#include <physics/CCPhysicsBody.h>

#include <Globals.h>
#include <Entities/Creep.h>
#include <2d/CCActionInterval.h>

USING_NS_CC;

bool Tower::init(std::string pBaseTexturePath, std::string pGunTexturePath, float pRangeRadius, float pCooldown,
                 unsigned int pCost) {
    if (!Node::init())
        return false;

    mCooldown = pCooldown;
    mCost = pCost;
    mVerbose = false;

    mBase = Sprite::create(pBaseTexturePath);

    mGun = Sprite::create(pGunTexturePath);
    mGun->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);

    mBase->setScale(0.5f);

    mRange = Sprite::create("textures/range.png");
    mRange->setColor(Color3B(113, 201, 55));
    mRange->setScale(2 * pRangeRadius / mRange->getContentSize().width);
    mRange->runAction(RepeatForever::create(RotateBy::create(2.f, 30.f)));

    mBody = PhysicsBody::createCircle(pRangeRadius);
    mBody->setCategoryBitmask(TOWER_RANGE_MASK);
    mBody->setContactTestBitmask(ENEMY_MASK);
    mBody->setCollisionBitmask(NULL_MASK);

    mTarget = nullptr;

    this->setPhysicsBody(mBody);

    this->setRotation(SPRITE_ANGLE);

    this->addChild(mRange);
    this->addChild(mBase);
    this->addChild(mGun);

    this->scheduleUpdate();

    return true;
}

void Tower::addTarget(Creep *pTarget) {
    mTargeList.pushBack(pTarget);
}

void Tower::removeTarget(Creep *pTarget) {
    auto found = mTargeList.find(pTarget);

    if (found != mTargeList.end())
        mTargeList.erase(found);
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
            mTargeList.eraseObject(mTarget);
            mTarget = nullptr;
            this->unschedule(CC_SCHEDULE_SELECTOR(Tower::shoot));
        }

        findTarget();
    }
}

bool Tower::isTargetValid() {
    if (mTarget != nullptr) {
        auto found = mTargeList.find(mTarget);

        return found != mTargeList.end() && !mTarget->isDead();
    }

    return false;
}

void Tower::findTarget() {
    if (!mTargeList.empty())
        mTarget = mTargeList.at(0);
}

void Tower::adaptRotation() {
    Vec2 diff = mTarget->getPosition() - this->getPosition();
    diff.normalize();

    auto angle = CC_RADIANS_TO_DEGREES(diff.getAngle());

    mGun->setRotation(-1 * angle);
}

void Tower::setVerbose(bool pVerbose) {
    mRange->setVisible(pVerbose);
    mVerbose = pVerbose;
}
