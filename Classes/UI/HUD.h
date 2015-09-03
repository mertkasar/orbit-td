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
    cocos2d::ui::Layout *mBottomPanel;

public:
    void init(cocos2d::Layer *pLayer, GameScene *pGameScene);
};

#endif //HUD_H
