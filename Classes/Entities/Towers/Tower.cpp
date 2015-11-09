#include <Entities/Towers/Tower.h>

#include <2d/CCSprite.h>
#include <2d/CCActionInterval.h>
#include <physics/CCPhysicsBody.h>

#include <Globals.h>
#include <Entities/Creep.h>

USING_NS_CC;

bool Tower::init(std::string pBaseTexturePath, std::string pGunTexturePath, float pRangeRadius, float pCooldown,
                 unsigned int pCost) {
    if (!Node::init())
        return false;

    mLevel = 1;

    mCooldown = pCooldown;
    mNextShooting = 0.f;
    mCost = pCost;
    mVerbose = true;

    mBase = Sprite::create(pBaseTexturePath);

    mGun = Sprite::create(pGunTexturePath);
    mGun->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);

    mBase->setScale(0.5f);

    mRange = Sprite::create("textures/range.png");
    mRange->setVisible(false);
    mRange->setColor(Color3B(113, 201, 55));
    mRange->setScale(2 * pRangeRadius / mRange->getContentSize().width);
    mRange->runAction(RepeatForever::create(RotateBy::create(2.f, 30.f)));

    mBody = PhysicsBody::createCircle(pRangeRadius);
    mBody->setEnable(false);
    mBody->setCategoryBitmask(TOWER_RANGE_MASK);
    mBody->setContactTestBitmask(ENEMY_MASK);
    mBody->setCollisionBitmask(NULL_MASK);

    mTarget = nullptr;

    this->addChild(mRange);
    this->addChild(mBase);
    this->addChild(mGun);

    this->setPhysicsBody(mBody);

    mRange->setOpacity(150);
    mBase->setOpacity(150);
    mGun->setOpacity(150);

    this->setRotation(SPRITE_ANGLE);

    return true;
}

void Tower::build() {
    mBody->setEnable(true);
    mRange->setOpacity(255);
    mBase->setOpacity(255);
    mGun->setOpacity(255);

    setVerbose(false);

    this->scheduleUpdate();
}

void Tower::addTarget(Creep *pTarget) {
    mTargetList.pushBack(pTarget);
}

void Tower::removeTarget(Creep *pTarget) {
    auto found = mTargetList.find(pTarget);

    if (found != mTargetList.end())
        mTargetList.erase(found);
}

void Tower::update(float pDelta) {
    if (isTargetValid()) {
        adaptRotation();

        mNextShooting = mNextShooting - pDelta;

        if (mNextShooting <= 0.f) {
            shoot(pDelta);
            mNextShooting = mCooldown;
        }
    } else {
        if (mTarget != nullptr) {
            mTargetList.eraseObject(mTarget);
            mTarget = nullptr;
            mNextShooting = 0.f;
        }

        findTarget();
    }
}

void Tower::upgrade() {
    mLevel++;
    auto scaleFactor = (float) (1 + mLevel * 0.1);
    //mBody->setScale(scaleFactor, scaleFactor);
    mRange->setScale(scaleFactor);
}

bool Tower::isTargetValid() {
    if (mTarget != nullptr) {
        auto found = mTargetList.find(mTarget);

        return found != mTargetList.end() && !mTarget->isDead();
    }

    return false;
}

void Tower::findTarget() {
    if (!mTargetList.empty())
        mTarget = mTargetList.at(0);
}

void Tower::adaptRotation() {
    Vec2 diff = mTarget->getPosition() - this->getPosition();
    diff.normalize();

    auto angle = CC_RADIANS_TO_DEGREES(diff.getAngle());

    mGun->setRotation(-1 * angle);
}

void Tower::setVerbose(bool pVerbose) {
    if (pVerbose) {
        mRange->setVisible(true);
        auto currentScale = mRange->getScale();
        mRange->setScale(0.f);
        mRange->runAction(ScaleTo::create(0.15f, currentScale));
    } else {
        mRange->setVisible(false);
    }

    mVerbose = pVerbose;
}

Tower::~Tower() {
    CCLOG("Tower deleted");
}
