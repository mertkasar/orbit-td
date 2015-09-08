#ifndef MAP_LAYER_H
#define MAP_LAYER_H

#include <2d/CCLayer.h>

class GameScene;

namespace cocos2d {
    namespace ui {
        class ImageView;
    }
}

class MapLayer : public cocos2d::Layer {
private:
    GameScene *mGameScene;

    std::map<cocos2d::Vec2, cocos2d::ui::ImageView *> mSlotMap;

private:
    MapLayer(GameScene *pGameScene);

public:
    static MapLayer *create(GameScene *pGameScene);

    virtual bool init();

    void activateSlot(cocos2d::Vec2 pTile);

    void deactivateSlot(cocos2d::Vec2 pTile);
};

#endif //MAP_LAYER_H
