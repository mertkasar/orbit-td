#ifndef WHEELMENU_H
#define WHEELMENU_H

#include <math/Vec2.h>
#include <ui/UILayout.h>
#include <ui/UIWidget.h>

namespace cocos2d {
    class Ref;

    class Node;

    class Layer;
}

class GameScene;

class WheelMenu {
private:
    GameScene *mGameScene;

    cocos2d::Vec2 mCurrentTile;

    bool mOpen;

    cocos2d::Node *mRoot;

    cocos2d::ui::Layout *mTowerMenu;

public:
    void init(cocos2d::Layer *pLayer, GameScene *pGameScene);

    void update(float pDelta);

    void openAt(cocos2d::Vec2 pPosition);

    void close();

    bool isOpen() const {
        return mOpen;
    }

private:
    void towerButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);
};

#endif //WHEELMENU_H
