#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <2d/CCLayer.h>

#include <Utilities/Path.h>
#include <Utilities/Grid.h>

#include <Utilities/Pool.h>
#include <UI/WheelMenu.h>
#include <UI/HUD.h>
#include <Scenes/WaveDirector.h>

namespace cocos2d {
    class DrawNode;

    class Label;
};

class Tower;

class Creep;

class GameScene : public cocos2d::Layer {
private:
    cocos2d::Size mVisibleSize;
    cocos2d::Vec2 mOrigin;
    cocos2d::Vec2 mCanvasCenter;

    cocos2d::LayerColor *mBackgroundLayer;
    cocos2d::Layer *mGameplayLayer;
    cocos2d::Layer *mUILayer;

    cocos2d::DrawNode *mPathCanvas;

    WaveDirector mWaveDirector;
    Pool<Creep> mCreepPool;
    cocos2d::Vector<Creep *> mCreeps;

    Grid mGrid;
    Path mPath;
    cocos2d::Vec2 mStart;
    cocos2d::Vec2 mGoal;

    WheelMenu mWheelMenu;
    HUD mHUD;

    unsigned int mTotalCoin;
    unsigned int mLife;

public:
    GameScene();

    ~GameScene();

    static cocos2d::Scene *createScene();

    virtual bool init();

    virtual void update(float pDelta);

    CREATE_FUNC(GameScene);

    void spawnEnemy(unsigned int pType, int pOrder);

    bool placeTower(unsigned int pType, cocos2d::Vec2 pTile);

    unsigned int getTotalCoin() const {
        return mTotalCoin;
    }

    unsigned int getRemainingLife() const {
        return mLife;
    }

    int getCreepCount() const {
        return (int) mCreeps.size();
    }

    WaveDirector &getWaveDirector() {
        return mWaveDirector;
    }

private:
    void buildScene();

    void connectListeners();

    bool isAvailable(const TraverseData &pTraversed, cocos2d::Vec2 pTile);

    void drawPath();
};

#endif // GAME_SCENE_H
