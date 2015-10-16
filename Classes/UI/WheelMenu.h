#ifndef WHEELMENU_H
#define WHEELMENU_H

#include <math/Vec2.h>
#include <ui/UILayout.h>
#include <ui/UIWidget.h>
#include <Globals.h>

namespace cocos2d {
    class Ref;

    class Node;

    class Layer;
}

class GameScene;

class WheelMenu {
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
    GameScene *mGameScene;

    cocos2d::Vec2 mCurrentTile;

    TowerTypes mSelectedType;

    State mState;

    unsigned int mLastCoin;

    cocos2d::Node *mRoot;

    cocos2d::ui::Layout *mPurchaseMenu;

    cocos2d::ui::Layout *mVerboseMenu;

    cocos2d::ui::Layout *mValidationMenu;

public:
    void init(cocos2d::Layer *pLayer, GameScene *pGameScene);

    void update(float pDelta);

    void setState(State pState);

    void openAt(cocos2d::Vec2 pPosition);

    void close();

    bool isOpen() const {
        return mState != IDLE;
    }

    cocos2d::Vec2 getCurrentTile() const {
        return mCurrentTile;
    }

private:
    void towerButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);

    void sellButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);
};

#endif //WHEELMENU_H
