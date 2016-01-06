#ifndef MAP_LAYER_H
#define MAP_LAYER_H

#include <2d/CCLayer.h>

#include <Utilities/Path.h>
#include <Utilities/Grid.h>
#include <Utilities/Algorithm.h>

class World;

namespace cocos2d {
    class Sprite;

    class DrawNode;
}

class MapLayer : public cocos2d::Layer {
private:
    MapLayer(World *world);

public:
    static MapLayer *create(World *world);

    virtual bool init();

    void activateSlot(cocos2d::Vec2 tile);

    void deactivateSlot(cocos2d::Vec2 tile);

    void setSlotColor(cocos2d::Vec2 tile, cocos2d::Color3B color);

    void updateMap(const TraverseData &traversed, cocos2d::Vec2 tile, int value);

    TraverseData traverseAgainst(cocos2d::Vec2 node, unsigned int value);

    bool isPathClear(const TraverseData &traversed);

    cocos2d::Vec2 getTouchedSlot(cocos2d::Vec2 location);

private:
    void drawPath();

private:
    World *_world;

    std::map<cocos2d::Vec2, cocos2d::Sprite *> _slotMap;
    cocos2d::DrawNode *_pathCanvas;

public:
    Grid _grid;
    Path _path;
    cocos2d::Vec2 _start;
    cocos2d::Vec2 _goal;
};

#endif //MAP_LAYER_H
