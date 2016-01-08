#ifndef WHEEL_MENU_H
#define WHEEL_MENU_H

#include "../Globals.h"

#include <math/Vec2.h>
#include <ui/UILayout.h>
#include <ui/UIWidget.h>

namespace cocos2d {
    class Ref;

    class Node;

    class Layer;
}

class World;

class TowerMock;

class WheelMenu : public cocos2d::Node {
public:
    enum ButtonTag {
        ACCEPT,
        DECLINE,
        UPGRADE,
        SELL
    };

    enum State {
        IDLE,
        PURCHASE,
        VERBOSE,
        VALIDATION
    };

public:
    static WheelMenu *create(World *world);

    bool init(World *world);

    virtual void update(float delta);

    void setState(State state);

    void openAt(cocos2d::Vec2 position);

    void close();

    bool isOpen() const {
        return _state != IDLE;
    }

private:
    void updateButtonStates();

    void towerButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

    void sellButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

    void upgradeButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

    void declineButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

private:
    World *_world;

    cocos2d::Vec2 _currentTile;

    ModelID _selectedType;

    State _state;

    unsigned int _lastCoin;

    cocos2d::Node *_rootMenu;

    cocos2d::ui::Layout *_purchaseMenu;

    cocos2d::ui::Layout *_verboseMenu;

    cocos2d::ui::Layout *_validationMenu;

    TowerMock *_mock;
};

#endif //WHEEL_MENU_H
