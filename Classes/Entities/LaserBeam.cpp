#include "LaserBeam.h"

#include "EnemyShip.h"

#include <2d/CCSprite.h>
#include <2d/CCSpriteFrame.h>

#define DIM 64.f

USING_NS_CC;

LaserBeam::LaserBeam() {
    CCLOG("LaserBeam created");
}

LaserBeam::~LaserBeam() {
    CCLOG("LaserBeam deleted");
}

bool LaserBeam::init() {
    if (!Node::init())
        return false;

    _start = Node::create();

    auto tile = Sprite::createWithSpriteFrameName("start_b.png");
    tile->setTag(0);
    _start->addChild(tile);

    tile = Sprite::createWithSpriteFrameName("start_f.png");
    tile->setTag(1);
    _start->addChild(tile);

    _mid = Node::create();

    tile = Sprite::createWithSpriteFrameName("mid_b.png");
    tile->setTag(0);
    tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _mid->addChild(tile);

    tile = Sprite::createWithSpriteFrameName("mid_f.png");
    tile->setTag(1);
    tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _mid->addChild(tile);

    _mid->setPosition(DIM / 2, 0);

    _end = Node::create();

    tile = Sprite::createWithSpriteFrameName("end_b.png");
    tile->setTag(0);
    tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _end->addChild(tile);

    tile = Sprite::createWithSpriteFrameName("end_f.png");
    tile->setTag(1);
    tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _end->addChild(tile);

    addChild(_start);
    addChild(_mid);
    addChild(_end);

    setCascadeOpacityEnabled(true);
    scheduleUpdate();

    return true;
}

void LaserBeam::update(cocos2d::Vec2 end) {
    auto diff = end - convertToWorldSpace(getPosition());
    auto ratio = (diff.length() - DIM) / DIM;

    _mid->setScaleX(ratio);
    _end->setPosition(diff.length() - DIM / 2.f, 0.f);
}

void LaserBeam::setColor(const Color3B &baseColor) {
    _start->getChildByTag(0)->setColor(baseColor);
    _mid->getChildByTag(0)->setColor(baseColor);
    _end->getChildByTag(0)->setColor(baseColor);
}