#include <Entities/Towers/Tower.h>

#include <2d/CCSprite.h>
#include <2d/CCActionInterval.h>
#include <physics/CCPhysicsBody.h>

#include <Entities/Creep.h>

USING_NS_CC;

#define COST_RATIO 0.1f
#define RANGE_RATIO 0.15f
#define DAMAGE_RATIO 0.1f
#define CD_RATIO -0.2f

bool Tower::init(TowerModel pModel) {
    if (!Node::init())
        return false;

    mModel = pModel;
    mCost = pModel.baseCost;
    mRange = pModel.baseRange;
    mDamage = pModel.baseDamage;
    mCooldown = pModel.baseCD;

    mLevel = 0;
    mNextShooting = 0.f;
    mVerbose = true;

    mGunSprite = Sprite::create(pModel.gunSpritePath);
    mGunSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

    mBaseSprite = Sprite::create("textures/tower_base.png");
    mBaseSprite->setScale(0.5f);
    mBaseSprite->setSpriteFrame(SpriteFrame::create("textures/tower_base.png", Rect(0, 0, 90, 90)));

    mRangeSprite = Sprite::create("textures/range.png");
    mRangeSprite->setVisible(false);
    mRangeSprite->setColor(Color::GREEN);
    mRangeSprite->setScale(2 * mRange / mRangeSprite->getContentSize().width);
    mRangeSprite->runAction(RepeatForever::create(RotateBy::create(2.f, 30.f)));

    mBody = createBody(mRange);
    mBody->setEnable(false);
    this->setPhysicsBody(mBody);

    mTarget = nullptr;

    this->addChild(mRangeSprite);
    this->addChild(mBaseSprite);
    this->addChild(mGunSprite);

    this->setCascadeOpacityEnabled(true);
    this->setOpacity(150);

    return true;
}

void Tower::build() {
    mBody->setEnable(true);
    this->setOpacity(255);
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

void Tower::upgrade(cocos2d::Color3B &pColor) {
    mLevel++;

    mCost = (unsigned int) (mCost + mCost * COST_RATIO);
    mRange = mRange + mRange * RANGE_RATIO;
    mDamage = mDamage + mDamage * DAMAGE_RATIO;
    mCooldown = mCooldown + mCooldown * CD_RATIO;

    mRangeSprite->setScale(mRange / mModel.baseRange);
    mRangeSprite->setColor(pColor);

    mBaseSprite->setSpriteFrame(SpriteFrame::create("textures/tower_base.png", Rect(mLevel * 90, 0, 90, 90)));

    mBody->removeFromWorld();
    mBody = createBody(mRange);
    this->setPhysicsBody(mBody);
}

bool Tower::isTargetValid() {
    if (mTarget != nullptr) {
        auto found = mTargetList.find(mTarget);

        return found != mTargetList.end() && !mTarget->isDead();
    }

    return false;
}

PhysicsBody *Tower::createBody(float pRangeRadius) {
    auto body = PhysicsBody::createCircle(pRangeRadius);

    body->setCategoryBitmask(TOWER_RANGE_MASK);
    body->setContactTestBitmask(ENEMY_MASK);
    body->setCollisionBitmask(NULL_MASK);

    return body;
}

void Tower::findTarget() {
    if (!mTargetList.empty())
        mTarget = mTargetList.at(0);
}

void Tower::adaptRotation() {
    Vec2 diff = mTarget->getPosition() - this->getPosition();
    diff.normalize();

    auto angle = CC_RADIANS_TO_DEGREES(diff.getAngle());

    mGunSprite->setRotation(-1 * angle);
}

void Tower::setVerbose(bool pVerbose) {
    if (pVerbose) {
        mRangeSprite->setVisible(true);
        auto currentScale = mRangeSprite->getScale();
        mRangeSprite->setScale(0.f);
        mRangeSprite->runAction(ScaleTo::create(0.15f, currentScale));
    } else {
        mRangeSprite->setVisible(false);
    }

    mVerbose = pVerbose;
}

Tower::~Tower() {
    CCLOG("Tower deleted");
}

const Color3B &Tower::getBaseColor() {
    return mRangeSprite->getColor();
}
