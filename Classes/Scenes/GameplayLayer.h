#ifndef GAMEPLAY_LAYER_H
#define GAMEPLAY_LAYER_H

#include <2d/CCLayer.h>
#include <Utilities/Pool.h>
#include <Globals.h>
#include <Entities/Missile.h>

class World;

class Tower;

class Creep;

class Path;

class GameplayLayer : public cocos2d::Layer {
private:
    World *mWorld;

    bool mPaused;

    Pool<Creep> mCreepPool;
    Pool<Missile> mMissilePool;

    cocos2d::Vector<Creep *> mCreeps;
    cocos2d::Vector<Missile *> mMissiles;

    std::map<cocos2d::Vec2, Tower *> mTowerMap;

    Tower *mMock;

private:
    GameplayLayer(World *pWorld);

public:
    static GameplayLayer *create(World *pWorld);

    virtual bool init();

    virtual void update(float pDelta);

    void addEnemy(CreepTypes pType, int pOrder, Path &pPath);

    void addMissile(cocos2d::Vec2 pPosition, const cocos2d::Color3B &pBaseColor, float pDamage, Creep *pTarget);

    void createMock(TowerTypes pType, cocos2d::Vec2 pTile);

    void removeMock();

    void buildMock(cocos2d::Vec2 pTile);

    Tower *getTower(cocos2d::Vec2 pTile);

    void deleteTower(cocos2d::Vec2 pTile);

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
