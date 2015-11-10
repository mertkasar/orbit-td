#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <2d/CCLayer.h>

#include <Utilities/Path.h>
#include <Utilities/Grid.h>

#include <Utilities/Pool.h>
#include <UI/WheelMenu.h>

namespace cocos2d {
    class DrawNode;

    class Label;

    class PhysicsWorld;
};

class Tower;

class Creep;

class MapLayer;

class GameplayLayer;

class HUDLayer;

class World : public cocos2d::Layer {
private:
    cocos2d::Size mVisibleSize;
    cocos2d::Vec2 mOrigin;
    cocos2d::Vec2 mCanvasCenter;

    cocos2d::PhysicsWorld *mPhysicsWorld;

    cocos2d::DrawNode *mPathCanvas;

    std::vector<std::vector<CreepTypes>> mWaves;
    unsigned int mCurrentWave;
    bool mCleared;

    cocos2d::Vec2 mStart;
    cocos2d::Vec2 mGoal;

    WheelMenu mWheelMenu;

    unsigned int mTotalCoin;

    unsigned int mLife;

public:
    cocos2d::LayerColor *backgroundLayer;
    MapLayer *mapLayer;
    GameplayLayer *gameplayLayer;
    HUDLayer *hudLayer;

    Grid mGrid;
    Path mPath;

public:
    World();

    ~World();

    static cocos2d::Scene *createScene();

    virtual bool init();

    virtual void update(float pDelta);

    CREATE_FUNC(World);

    bool placeTower(TowerTypes pType, cocos2d::Vec2 pTile);

    void destroyTower(cocos2d::Vec2 pTile);

    void upgradeTower(cocos2d::Vec2 pTile);

    bool spawnNextWave();

    void balanceTotalCoin(int pBalance) {
        mTotalCoin = mTotalCoin + pBalance;
    }

    unsigned int getTotalCoin() const {
        return mTotalCoin;
    }

    void balanceRemainingLife(int pBalance) {
        mLife = mLife + pBalance;
    }

    unsigned int getRemainingLife() const {
        return mLife;
    }

    cocos2d::PhysicsWorld *getPhysicsWorld() const {
        return mPhysicsWorld;
    }

    void setPhysicsWorld(cocos2d::PhysicsWorld *pPhysicsWorld) {
        mPhysicsWorld = pPhysicsWorld;
    }

    bool isCleared() {
        return mCleared;
    }

private:
    void buildScene();

    void connectListeners();

    bool isAvailable(const TraverseData &pTraversed, cocos2d::Vec2 pTile);

    void drawPath();
};

#endif // GAME_SCENE_H
