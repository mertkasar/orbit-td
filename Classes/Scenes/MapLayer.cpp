#include <Scenes/MapLayer.h>

#include <2d/CCDrawNode.h>
#include <2d/CCActionInterval.h>
#include <ui/UIImageView.h>

#include "World.h"
#include <Utilities/Algorithm.h>

USING_NS_CC;

const Color3B COLOR_ACTIVE(Color3B(113, 201, 55));
const Color3B COLOR_EMPTY(Color3B(108, 115, 131));
const Color3B COLOR_LOCKED(Color3B(54, 58, 66));

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

    //Draw Planet
    auto planet = DrawNode::create();
    planet->drawSolidCircle(Vec2(-480.f, 360.f), 600.f, 0.f, 50, Color4F::BLUE);
    this->addChild(planet);

    auto &grid = mWorld->mGrid;

    Vec2 size = grid.getSize();
    for (int j = 0; j < size.y; j++) {
        ui::ImageView *orbit = ui::ImageView::create("textures/ui/orbit.png");
        orbit->setColor(COLOR_EMPTY);
        orbit->setPosition(algorithm::toCircularGrid(Vec2(2, j)));
        this->addChild(orbit);
    }

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            if (grid.getNode(Vec2(i, j)) == 0) {
                Vec2 position = algorithm::toCircularGrid(Vec2(i, j));

                ui::ImageView *shadow = ui::ImageView::create("textures/ui/touch_shadow.png");
                shadow->setPosition(position);
                this->addChild(shadow);

                ui::ImageView *touchArea = ui::ImageView::create("textures/ui/touch.png");
                touchArea->setColor(COLOR_EMPTY);
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

    ui::ImageView *slot = found->second;
    slot->setColor(COLOR_ACTIVE);
    slot->runAction(RepeatForever::create(RotateBy::create(2.f, 30.f)));
}

void MapLayer::deactivateSlot(cocos2d::Vec2 pTile) {
    auto found = mSlotMap.find(pTile);

    assert(found != mSlotMap.end());

    ui::ImageView *slot = found->second;
    slot->setColor(COLOR_EMPTY);
    slot->stopAllActions();
    slot->setRotation(0.f);
}
