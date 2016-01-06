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
public:
    static HUDLayer *create(World *world);

    bool init(World *world);

    virtual void update(float delta);

    void notify(char type, std::string message, float duration = 1.f);

private:
    void menuButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

    void nextButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

    void pauseButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

    void ffButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

private:
    World *_world;

    cocos2d::ui::Layout *_topPanel;
    cocos2d::ui::Layout *_notificationPanel;
    cocos2d::ui::Layout *_bottomPanel;
};

#endif //HUD_LAYER_H
