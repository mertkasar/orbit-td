#include <Entities/Creep.h>

#include <2d/CCSprite.h>
#include <2d/CCAction.h>
#include <physics/CCPhysicsBody.h>
#include <Utilities/SteeringDirector.h>
#include <base/CCValue.h>

USING_NS_CC;

#define HBAR_WIDTH 2.f
#define HBAR_HEIGHT 8.f

Creep::Creep() {
    CCLOG("Creep created");
}

Creep::~Creep() {
    CCLOG("Creep deleted");
}

bool Creep::init() {
    if (!Node::init())
        return false;

    mKilled = false;
    mReachedEnd = false;

    mSprite = Sprite::create();
    mSprite->setContentSize(Size(100, 90));

    mBody = PhysicsBody::createCircle(mSprite->getContentSize().width / 2.f);
    mBody->setCategoryBitmask(ENEMY_MASK);
    mBody->setContactTestBitmask(TOWER_RANGE_MASK);
    mBody->setCollisionBitmask(NULL_MASK);
    this->setPhysicsBody(mBody);

    mHPBar = Node::create();
    mHPBar->setPosition(Vec2(80.f, -40.f));

    auto hpBarB = Sprite::createWithSpriteFrameName("hp_bar.png");
    hpBarB->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    hpBarB->setName("bg");
    hpBarB->setColor(Color::RED);
    hpBarB->setScale(HBAR_WIDTH, HBAR_HEIGHT);
    mHPBar->addChild(hpBarB);

    auto hpBarF = Sprite::createWithSpriteFrameName("hp_bar.png");
    hpBarF->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    hpBarF->setName("fg");
    hpBarF->setColor(Color::GREEN);
    hpBarF->setScale(HBAR_WIDTH, HBAR_HEIGHT);
    mHPBar->addChild(hpBarF);

    this->addChild(mSprite);
    this->addChild(mHPBar);

    return true;
}

void Creep::update(float pDelta) {
    WayPoint target = mPath.getNextWaypoint();
    float reachRadius = target.reachRadius + mSprite->getContentSize().width / 2.f;
    if (this->getPosition().distance(target.location) <= reachRadius) {
        if (mPath.eop())
            mReachedEnd = true;
        else {
            mPath.forward();
            target = mPath.getNextWaypoint();
        }
    }

    SteeringDirector::getInstance()->seek(this, target.location);

    // Adapt rotation
    auto angle = CC_RADIANS_TO_DEGREES(mBody->getVelocity().getAngle());
    mSprite->setRotation(-angle);

    if (mCurrentHP <= 0.f)
        mKilled = true;
}

void Creep::restart(const cocos2d::ValueMap &pModel, cocos2d::Vec2 pPosition, const Path &pPath) {
    mKilled = false;
    mReachedEnd = false;

    mSprite->initWithSpriteFrameName(pModel.at("sprite").asString());
    mMaxHP = pModel.at("max_hp").asFloat();
    mReward = pModel.at("reward").asFloat();
    mBody->setMass(pModel.at("mass").asFloat());
    mBody->setVelocityLimit(pModel.at("max_velocity").asFloat());

    mCurrentHP = mMaxHP;

    this->setPosition(pPosition);
    this->setScale(0.5f);
    mBody->setVelocity(Vec2::ZERO);

    mPath.clone(pPath);

    updateHPBar();

    this->scheduleUpdate();
}

void Creep::deal(float pDamage) {
    mCurrentHP = mCurrentHP - pDamage;

    if (mCurrentHP < 0.f)
        mCurrentHP = 0.f;

    updateHPBar();
}

void Creep::updateHPBar() {
    auto fg = mHPBar->getChildByName("fg");
    auto ratio = mCurrentHP / mMaxHP;
    fg->setScaleY(HBAR_HEIGHT * ratio);
}
