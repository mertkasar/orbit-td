#include "TowerMock.h"

#include <2d/CCSprite.h>
#include <2d/CCActionInterval.h>

#define OPACITY 150.f

USING_NS_CC;

bool TowerMock::init() {
    if (!Node::init())
        return false;

    auto sprite = Sprite::createWithSpriteFrameName("base_0.png");
    sprite->setScale(0.5f);
    addChild(sprite);

    sprite = Sprite::create();
    sprite->setName("gun");
    addChild(sprite);

    sprite = Sprite::createWithSpriteFrameName("range.png");
    sprite->setColor(Palette::GREEN);
    sprite->runAction(RepeatForever::create(RotateBy::create(2.f, 30.f)));
    addChild(sprite);

    setVisible(false);
    setCascadeOpacityEnabled(true);
    setOpacity((GLubyte) OPACITY);

    return true;
}

void TowerMock::update(const cocos2d::ValueMap &model) {
    auto gunSprite = static_cast<Sprite*>(getChildByName("gun"));

    gunSprite->initWithSpriteFrameName(model.at("gun_sprite_name").asString());
    gunSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    gunSprite->setOpacity((GLubyte) OPACITY);
}
