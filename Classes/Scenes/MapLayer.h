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
    World *mWorld;

    std::map<cocos2d::Vec2, cocos2d::Sprite *> mSlotMap;
    cocos2d::DrawNode *mPathCanvas;

public:
    Grid mGrid;
    Path mPath;
    cocos2d::Vec2 mStart;
    cocos2d::Vec2 mGoal;

private:
    MapLayer(World *pWorld);

public:
    static MapLayer *create(World *pWorld);

    virtual bool init();

    void activateSlot(cocos2d::Vec2 pTile);

    void deactivateSlot(cocos2d::Vec2 pTile);

    void setSlotColor(cocos2d::Vec2 pTile, cocos2d::Color3B pColor);

    void updateMap(const TraverseData &pTraversed, cocos2d::Vec2 pTile, int pValue);

    TraverseData traverseAgainst(cocos2d::Vec2 pNode, unsigned int pValue);

    bool isPathClear(const TraverseData &pTraversed);

    cocos2d::Vec2 getTouchedSlot(cocos2d::Vec2 pLocation);

private:
    void drawPath();
};

#endif //MAP_LAYER_H
