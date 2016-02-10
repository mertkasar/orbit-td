#ifndef HUD_LAYER_H
#define HUD_LAYER_H

#include <math/Vec2.h>
#include <2d/CCLayer.h>
#include <ui/UIWidget.h>

namespace cocos2d {
    class Ref;

    namespace ui {
        class Layout;
    }
}

class World;

class DialogBox;

class HUDLayer : public cocos2d::Layer {
private:
    HUDLayer(World *world);

    ~HUDLayer();

public:
    static HUDLayer *create(World *world);

    virtual bool init();

    virtual void update(float delta);

    void notify(char type, std::string message, float duration = 1.f);

    void updateLife();

    void show(float delay = 0.f);

    void hide(float delay = 0.f);

    void close(float delay = 0.f);

private:
    void menuButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

    void nextButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

    void pauseButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

private:
    World *_world;

    cocos2d::ui::Layout *_topPanel;
    cocos2d::ui::Layout *_notificationPanel;
    cocos2d::ui::Layout *_bottomPanel;

    cocos2d::ui::Layout *_shieldBar;
    cocos2d::ui::Layout *_energy;

    DialogBox *_dialogBox;
};

#endif //HUD_LAYER_H
