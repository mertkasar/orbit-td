#ifndef HELLOWORLD_SCENE_H
#define HELLOWORLD_SCENE_H

#include <2d/CCLayer.h>
#include <Utilities/Path.h>

class Enemy;

class GameScene : public cocos2d::Layer {
private:
    cocos2d::Size mVisibleSize;
    cocos2d::Vec2 mOrigin;
    cocos2d::Vec2 mCanvasCenter;

    cocos2d::LayerColor *mBackgroundLayer;
    cocos2d::Layer *mGameplayLayer;

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
};

#endif // HELLOWORLD_SCENE_H
