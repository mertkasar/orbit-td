#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <2d/CCLayer.h>

#include <Utilities/Path.h>
#include <Utilities/Grid.h>

#include <Globals.h>

namespace cocos2d {
    class DrawNode;
};

class Enemy;

class GameScene : public cocos2d::Layer {
private:
    cocos2d::Size mVisibleSize;
    cocos2d::Vec2 mOrigin;
    cocos2d::Vec2 mCanvasCenter;

    cocos2d::LayerColor *mBackgroundLayer;
    cocos2d::Layer *mGameplayLayer;

    cocos2d::DrawNode *mPathCanvas;

    cocos2d::Vector<Enemy *> mEnemies;

    Grid mGrid;
    Path mPath;

public:
    GameScene();

    ~GameScene();

    static cocos2d::Scene *createScene();

    virtual bool init();

    CREATE_FUNC(GameScene);

private:
    void buildScene();

    void connectListeners();

    void spawnEnemy(float pDelta);

    bool isAvailable(const TraverseData &pTraversed, cocos2d::Vec2 pTile);

    void placeTower(cocos2d::Vec2 pTile);

    void drawPath();
};

#endif // GAME_SCENE_H
