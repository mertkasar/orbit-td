#ifndef HUD_H
#define HUD_H

#include <math/Vec2.h>
#include <ui/UILayout.h>
#include <ui/UIWidget.h>

namespace cocos2d {
    class Ref;

    class Node;

    class Layer;
}

class GameScene;

class HUD {
private:
    GameScene *mGameScene;

    cocos2d::ui::Layout *mTopPanel;
    cocos2d::ui::Layout *mNotificationPanel;
    cocos2d::ui::Layout *mBottomPanel;

public:
    void init(cocos2d::Layer *pLayer, GameScene *pGameScene);

    void update(float pDelta);

    void notify(char pType, std::string pMessage, float pDuration = 1.f);

private:
    void menuButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);

    void nextButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);

    void pauseButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);

    void ffButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType);
};

#endif //HUD_H
