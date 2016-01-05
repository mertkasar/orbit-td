#ifndef WHEELMENU_H
#define WHEELMENU_H

#include <math/Vec2.h>
#include <ui/UILayout.h>
#include <ui/UIWidget.h>

#include <Globals.h>

class TowerMock;

namespace cocos2d {
    class Ref;

    class Node;

    class Layer;
}

class World;

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

private:
    World *mWorld;

    cocos2d::Vec2 mCurrentTile;

    TowerTypes mSelectedType;

    State mState;

    unsigned int mLastCoin;

    cocos2d::Node *mRootMenu;

    cocos2d::ui::Layout *mPurchaseMenu;

    cocos2d::ui::Layout *mVerboseMenu;

    cocos2d::ui::Layout *mValidationMenu;

    TowerMock *mMock;

public:
    static WheelMenu *create(World *pWorld);

    bool init(World *pWorld);

    virtual void update(float pDelta);

    void setState(State pState);

    void openAt(cocos2d::Vec2 pPosition);

    void close();

    bool isOpen() const {
        return mState != IDLE;
    }

private:
    void updateButtonStates();

    void towerButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);

    void sellButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);

    void upgradeButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);

    void declineButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);
};

#endif //WHEELMENU_H
