#include <Scenes/MapLayer.h>

#include <2d/CCDrawNode.h>
#include <2d/CCActionInterval.h>
#include <2d/CCSprite.h>

#include <Scenes/World.h>
#include <2d/CCSpriteFrameCache.h>

USING_NS_CC;

MapLayer::MapLayer(World *pWorld) {
    mWorld = pWorld;
}

MapLayer *MapLayer::create(World *pWorld) {
    MapLayer *layer = new(std::nothrow) MapLayer(pWorld);

    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    } else {
        delete layer;
        layer = NULL;
        return NULL;
    }
}

bool MapLayer::init() {
    if (!Layer::init())
        return false;

    this->setName("map_layer");

    auto spriteCache = SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile("textures/spritesheets/map_layer.plist");

    //Draw Planet
    auto planet = DrawNode::create();
    planet->drawSolidCircle(Vec2(-480.f, 360.f), 600.f, 0.f, 50, Color4F::BLUE);
    this->addChild(planet);

    auto &grid = mWorld->mGrid;

    Vec2 size = grid.getSize();
    for (int j = 0; j < size.y; j++) {
        Sprite *orbit = Sprite::createWithSpriteFrameName("orbit.png");
        orbit->setColor(Color::GREY);
        orbit->setPosition(algorithm::toCircularGrid(Vec2(2, j)));
        this->addChild(orbit);
    }

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            if (grid.getNode(Vec2(i, j)) == 0) {
                Vec2 position = algorithm::toCircularGrid(Vec2(i, j));

                Sprite *shadow = Sprite::createWithSpriteFrameName("touch_shadow.png");
                shadow->setColor(Color::BG);
                shadow->setPosition(position);
                this->addChild(shadow);

                Sprite *touchArea = Sprite::createWithSpriteFrameName("touch.png");
                touchArea->setColor(Color::GREY);
                touchArea->setPosition(position);

                mSlotMap.insert(std::make_pair(Vec2(i, j), touchArea));
                this->addChild(touchArea);
            }
        }
    }

    return true;
}

void MapLayer::activateSlot(cocos2d::Vec2 pTile) {
    auto found = mSlotMap.find(pTile);

    assert(found != mSlotMap.end());

    Sprite *slot = found->second;
    slot->setColor(Color::GREEN);
    slot->runAction(RepeatForever::create(RotateBy::create(2.f, 30.f)));
}

void MapLayer::deactivateSlot(cocos2d::Vec2 pTile) {
    auto found = mSlotMap.find(pTile);

    assert(found != mSlotMap.end());

    Sprite *slot = found->second;
    slot->setColor(Color::GREY);
    slot->stopAllActions();
    slot->setRotation(0.f);
}

void MapLayer::setSlotColor(cocos2d::Vec2 pTile, cocos2d::Color3B pColor) {
    auto found = mSlotMap.find(pTile);

    assert(found != mSlotMap.end());

    Sprite *slot = found->second;
    slot->setColor(pColor);
}
