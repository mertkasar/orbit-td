#ifndef MAP_LAYER_H
#define MAP_LAYER_H

#include <2d/CCLayer.h>

class World;

namespace cocos2d {
    class Sprite;
}

class MapLayer : public cocos2d::Layer {
private:
    World *mWorld;

    std::map<cocos2d::Vec2, cocos2d::Sprite *> mSlotMap;

private:
    MapLayer(World *pWorld);

public:
    static MapLayer *create(World *pWorld);

    virtual bool init();

    void activateSlot(cocos2d::Vec2 pTile);

    void deactivateSlot(cocos2d::Vec2 pTile);

    void setSlotColor(cocos2d::Vec2 pTile, cocos2d::Color3B pColor);
};

#endif //MAP_LAYER_H
