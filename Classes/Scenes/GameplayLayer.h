#ifndef GAMEPLAY_LAYER_H
#define GAMEPLAY_LAYER_H

#include <2d/CCLayer.h>
#include <Utilities/Pool.h>
#include <Globals.h>

class GameScene;

class Tower;

class Creep;

class Path;

class GameplayLayer : public cocos2d::Layer {
private:
    GameScene *mGameScene;

    Pool<Creep> mCreepPool;

    cocos2d::Vector<Creep *> mCreeps;

    std::map<cocos2d::Vec2, Tower *> mTowerMap;

private:
    GameplayLayer(GameScene *pGameScene);

public:
    static GameplayLayer *create(GameScene *pGameScene);

    virtual bool init();

    virtual void update(float pDelta);

    void addEnemy(CreepTypes pType, int pOrder, Path& pPath);

    void addTower(TowerTypes pType, cocos2d::Vec2 pTile);

    cocos2d::Vector<Creep *> &getCreepList() {
        return mCreeps;
    }
};

#endif //GAMEPLAY_LAYER_H
