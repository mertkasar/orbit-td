#ifndef MAIN_MENU_LAYER_H
#define MAIN_MENU_LAYER_H

#include <2d/CCLayer.h>
#include <ui/UIWidget.h>

namespace cocos2d {
    class Ref;

    namespace ui {
        class Layout;
    }
}

class World;

class MainMenuLayer : public cocos2d::Layer {
private:
    MainMenuLayer(World *world);

    ~MainMenuLayer();

public:
    static MainMenuLayer *create(World *world);

    virtual bool init();

    void show(float delay = 0);

    void hide(float delay = 0);

    void close(float delay = 0);

private:
    void startButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

    void soundButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

    void quitButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

private:
    World *_world;

    bool _muted;
};

#endif // MAIN_MENU_LAYER_H
