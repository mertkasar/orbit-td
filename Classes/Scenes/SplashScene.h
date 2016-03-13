#ifndef SPLASH_SCENE_H
#define SPLASH_SCENE_H

#include <2d/CCLayer.h>

class SplashScene : public cocos2d::LayerColor {
private:
    SplashScene();

    ~SplashScene();

public:
    static cocos2d::Scene *createScene();

    virtual bool init();

    CREATE_FUNC(SplashScene);

private:
    void loadResources();
};

#endif //SPLASH_SCENE_H
