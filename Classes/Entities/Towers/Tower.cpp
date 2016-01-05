#include <Entities/Towers/Tower.h>

#include <2d/CCSprite.h>
#include <2d/CCActionInterval.h>
#include <physics/CCPhysicsBody.h>

#include <Entities/Creep.h>
#include <sstream>
#include <audio/include/SimpleAudioEngine.h>

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

    mGunSprite = Sprite::createWithSpriteFrameName(pModel.gunSpritePath);
    mGunSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

    mMuzzlePoint = Node::create();
    auto size = mGunSprite->getContentSize();
    mMuzzlePoint->setPosition(size.width, size.height / 2.f);

    mBaseSprite = Sprite::createWithSpriteFrameName("base_0.png");
    mBaseSprite->setScale(0.5f);

    mRangeSprite = Sprite::createWithSpriteFrameName("range.png");
    mRangeSprite->setVisible(false);
    mRangeSprite->setColor(Color::GREEN);
    mRangeSprite->setScale(2 * mRange / mRangeSprite->getContentSize().width);
    mRangeSprite->runAction(RepeatForever::create(RotateBy::create(2.f, 30.f)));

    mBody = createBody(mRange);
    this->setPhysicsBody(mBody);

    mTarget = nullptr;

    this->addChild(mRangeSprite);
    this->addChild(mBaseSprite);

    mGunSprite->addChild(mMuzzlePoint);
    this->addChild(mGunSprite);

    this->scheduleUpdate();

    return true;
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

    std::stringstream ss;
    ss << "base_" << mLevel << ".png";

    mBaseSprite->initWithSpriteFrameName(ss.str());

    mBody->removeFromWorld();
    mBody = createBody(mRange);
    this->setPhysicsBody(mBody);

    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/upgrade.wav");
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
