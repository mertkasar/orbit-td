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

    mStart = Node::create();

    auto tile = Sprite::createWithSpriteFrameName("start_b.png");
    tile->setTag(0);
    mStart->addChild(tile);

    tile = Sprite::createWithSpriteFrameName("start_f.png");
    tile->setTag(1);
    mStart->addChild(tile);

    mMid = Node::create();

    tile = Sprite::createWithSpriteFrameName("mid_b.png");
    tile->setTag(0);
    tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    mMid->addChild(tile);

    tile = Sprite::createWithSpriteFrameName("mid_f.png");
    tile->setTag(1);
    tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    mMid->addChild(tile);

    mMid->setPosition(DIM / 2, 0);

    mEnd = Node::create();

    tile = Sprite::createWithSpriteFrameName("end_b.png");
    tile->setTag(0);
    tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    mEnd->addChild(tile);

    tile = Sprite::createWithSpriteFrameName("end_f.png");
    tile->setTag(1);
    tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    mEnd->addChild(tile);

    this->addChild(mStart);
    this->addChild(mMid);
    this->addChild(mEnd);

    this->setCascadeOpacityEnabled(true);
    this->scheduleUpdate();

    return true;
}

void Beam::update(cocos2d::Vec2 pEnd) {
    auto diff = pEnd - this->convertToWorldSpace(this->getPosition());
    auto ratio = (diff.length() - DIM) / DIM;

    mMid->setScaleX(ratio);
    mEnd->setPosition(diff.length() - DIM / 2.f, 0.f);
}

void Beam::setColor(const Color3B &pBaseColor) {
    mStart->getChildByTag(0)->setColor(pBaseColor);
    mMid->getChildByTag(0)->setColor(pBaseColor);
    mEnd->getChildByTag(0)->setColor(pBaseColor);
}