#include <Entities/Beam.h>

#include <2d/CCSprite.h>
#include <Entities/Creep.h>
#include <2d/CCSpriteFrame.h>

USING_NS_CC;

#define DIM 32.f

Beam::Beam() {
    CCLOG("Beam created");
}

Beam::~Beam() {
    CCLOG("Beam deleted");
}

bool Beam::init() {
    if (!Node::init())
        return false;

    mStartB = Sprite::create("textures/laser.png");
    mStartB->setSpriteFrame(SpriteFrame::create("textures/laser.png", Rect(0, DIM, DIM, DIM)));
    this->addChild(mStartB);

    mStartO = Sprite::create("textures/laser.png");
    mStartO->setSpriteFrame(SpriteFrame::create("textures/laser.png", Rect(DIM, DIM, DIM, DIM)));
    this->addChild(mStartO);

    mMidB = Sprite::create("textures/laser.png");
    mMidB->setSpriteFrame(SpriteFrame::create("textures/laser.png", Rect(0, 0, DIM, DIM)));
    mMidB->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    mMidB->setPosition(DIM / 2, 0);
    this->addChild(mMidB);

    mMidO = Sprite::create("textures/laser.png");
    mMidO->setSpriteFrame(SpriteFrame::create("textures/laser.png", Rect(DIM, 0, DIM, DIM)));
    mMidO->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    mMidO->setPosition(DIM / 2, 0);
    this->addChild(mMidO);

    this->setCascadeOpacityEnabled(true);

    this->scheduleUpdate();

    return true;
}

void Beam::update(cocos2d::Vec2 pEnd) {
    auto diff = pEnd - this->convertToWorldSpace(this->getPosition());
    auto ratio = diff.length() / DIM + 1.f;

    mMidB->setScaleX(ratio);
    mMidO->setScaleX(ratio);

    //this->setRotation(-CC_RADIANS_TO_DEGREES(diff.getAngle()));
}

void Beam::setColor(const Color3B &pBaseColor) {
    mStartB->setColor(pBaseColor);
    mMidB->setColor(pBaseColor);
}