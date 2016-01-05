#ifndef HUD_LAYER_H
#define HUD_LAYER_H

#include <math/Vec2.h>
#include <ui/UILayout.h>
#include <ui/UIWidget.h>
#include <2d/CCLayer.h>

namespace cocos2d {
    class Ref;

    class Node;

    class Layer;
}

class World;

class HUDLayer : public cocos2d::Layer {
private:
    World *mWorld;

    cocos2d::ui::Layout *mTopPanel;
    cocos2d::ui::Layout *mNotificationPanel;
    cocos2d::ui::Layout *mBottomPanel;

public:
    static HUDLayer *create(World *pWorld);

    bool init(World *pWorld);

    virtual void update(float pDelta);

    void notify(char pType, std::string pMessage, float pDuration = 1.f);

private:
    void menuButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);

    void nextButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);

    void pauseButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);

    void ffButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);
};

#endif //HUD_LAYER_H
