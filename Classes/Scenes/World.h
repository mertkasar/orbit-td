#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <2d/CCLayer.h>
#include <base/CCValue.h>
#include <Globals.h>

namespace cocos2d {
    class DrawNode;

    class Label;

    class PhysicsWorld;
};

namespace CocosDenshion {
    class SimpleAudioEngine;
}

class Tower;

class Creep;

class MapLayer;

class GameplayLayer;

class HUDLayer;

class WheelMenu;

class World : public cocos2d::Layer {
private:
    cocos2d::Size mVisibleSize;
    cocos2d::Vec2 mOrigin;
    cocos2d::Vec2 mCanvasCenter;

    cocos2d::PhysicsWorld *mPhysicsWorld;

    std::unordered_map<unsigned int, cocos2d::ValueMap> mModels;

    std::vector<std::vector<ModelID>> mWaves;
    unsigned int mCurrentWave;
    bool mCleared;

    WheelMenu *mWheelMenu;

    unsigned int mTotalCoin;

    unsigned int mLife;

public:
    CocosDenshion::SimpleAudioEngine *audioEngine;

    cocos2d::LayerColor *backgroundLayer;
    MapLayer *mapLayer;
    GameplayLayer *gameplayLayer;
    HUDLayer *hudLayer;

    std::vector<cocos2d::Color3B> colors;

public:
    World();

    ~World();

    static cocos2d::Scene *createScene();

    virtual bool init();

    virtual void update(float pDelta);

    CREATE_FUNC(World);

    bool placeTower(ModelID pType, cocos2d::Vec2 pTile);

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

    const cocos2d::ValueMap &getModel(unsigned int pID) const {
        return mModels.at(pID);
    }

    void setPhysicsWorld(cocos2d::PhysicsWorld *pPhysicsWorld) {
        mPhysicsWorld = pPhysicsWorld;
    }

    bool isCleared() {
        return mCleared;
    }

private:
    void loadResources();

    void buildScene();

    void connectListeners();

    void loadModel(std::string pPath);
};

#endif // GAME_SCENE_H
