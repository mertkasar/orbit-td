#include <Entities/Beam.h>

#include <2d/CCSprite.h>
#include <Entities/Creep.h>
#include <2d/CCSpriteFrame.h>

USING_NS_CC;

#define DIM 64.f

Beam::Beam() {
    CCLOG("Beam created");
}

Beam::~Beam() {
    CCLOG("Beam deleted");
}

bool Beam::init() {
    if (!Node::init())
        return false;

    mStartB = Sprite::createWithSpriteFrameName("start_b.png");
    this->addChild(mStartB);

    mStartF = Sprite::createWithSpriteFrameName("start_f.png");
    this->addChild(mStartF);

    mMidB = Sprite::createWithSpriteFrameName("mid_b.png");
    mMidB->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    mMidB->setPosition(DIM / 2, 0);
    this->addChild(mMidB);

    mMidF = Sprite::createWithSpriteFrameName("mid_f.png");
    mMidF->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    mMidF->setPosition(DIM / 2, 0);
    this->addChild(mMidF);

    this->setCascadeOpacityEnabled(true);

    this->scheduleUpdate();

    return true;
}

void Beam::update(cocos2d::Vec2 pEnd) {
    auto diff = pEnd - this->convertToWorldSpace(this->getPosition());
    auto ratio = diff.length() / DIM - 0.5f;

    mMidB->setScaleX(ratio);
    mMidF->setScaleX(ratio);
}

void Beam::setColor(const Color3B &pBaseColor) {
    mStartB->setColor(pBaseColor);
    mMidB->setColor(pBaseColor);
}