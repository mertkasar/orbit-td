#ifndef GAMEPLAY_LAYER_H
#define GAMEPLAY_LAYER_H

#include <2d/CCLayer.h>
#include <Utilities/Pool.h>
#include <Globals.h>

class World;

class Tower;

class Creep;

class Path;

class GameplayLayer : public cocos2d::Layer {
private:
    World *mWorld;

    bool mPaused;

    Pool<Creep> mCreepPool;

    cocos2d::Vector<Creep *> mCreeps;

    std::map<cocos2d::Vec2, Tower *> mTowerMap;

    Tower *mMock;

private:
    GameplayLayer(World *pWorld);

public:
    static GameplayLayer *create(World *pWorld);

    virtual bool init();

    virtual void update(float pDelta);

    void addEnemy(CreepTypes pType, int pOrder, Path &pPath);

    void createMock(TowerTypes pType, cocos2d::Vec2 pTile);

    void removeMock();

    void buildMock(cocos2d::Vec2 pTile);

    Tower *getTower(cocos2d::Vec2 pTile);

    void deleteTower(cocos2d::Vec2 pTile);

    void upgradeTower(cocos2d::Vec2 pTile);

    void pauseScene();

    void resumeScene();

    bool isPaused() const {
        return mPaused;
    }

    cocos2d::Vector<Creep *> &getCreepList() {
        return mCreeps;
    }
};

#endif //GAMEPLAY_LAYER_H
