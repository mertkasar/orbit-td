#ifndef HELLOWORLD_SCENE_H
#define HELLOWORLD_SCENE_H

#include <2d/CCLayer.h>

class GameScene : public cocos2d::Layer {
private:
    cocos2d::Size mVisibleSize;
    cocos2d::Vec2 mOrigin;

    cocos2d::LayerColor *mBackgroundLayer;

public:
    GameScene();

    ~GameScene();

    static cocos2d::Scene *createScene();

    virtual bool init();

    CREATE_FUNC(GameScene);

private:
    void buildScene();
};

#endif // HELLOWORLD_SCENE_H
