#include "Planet.h"

#include <2d/CCSprite.h>
#include <2d/CCActionInterval.h>

USING_NS_CC;

bool Planet::init() {
    if (!Node::init())
        return false;

    auto sprite = Sprite::create("textures/planet_atmosphere.png");
    addChild(sprite);

    sprite = Sprite::create("textures/planet_base.png");
    sprite->runAction(RepeatForever::create(RotateBy::create(1.f, 3.f)));
    addChild(sprite);

    sprite = Sprite::create("textures/planet_overlay.png");
    addChild(sprite);

    return true;
}
