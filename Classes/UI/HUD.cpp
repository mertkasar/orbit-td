#include <UI/HUD.h>

#include <2d/CCMenu.h>
#include <ui/UIImageView.h>
#include <ui/UIButton.h>

#include <Scenes/GameScene.h>

USING_NS_CC;

void HUD::init(Layer *pLayer, GameScene *pGameScene) {
    mGameScene = pGameScene;

    mTopPanel = ui::Layout::create();
    mTopPanel->setBackGroundImage("textures/ui/top_panel.png");
    mTopPanel->setContentSize(mTopPanel->getBackGroundImageTextureSize());
    mTopPanel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    mTopPanel->setPosition(Vec2(0, 720));

    auto item = ui::Button::create("textures/ui/btn_next.png", "");
    item->setName("next_button");
    item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    item->setPosition(Vec2(100, mTopPanel->getContentSize().height / 2.f));
    mTopPanel->addChild(item);

    item = ui::Button::create("textures/ui/btn_menu.png", "");
    item->setName("menu_button");
    item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    item->setPosition(Vec2(mTopPanel->getContentSize().width  - 100, mTopPanel->getContentSize().height / 2.f));
    mTopPanel->addChild(item);

    mBottomPanel = ui::Layout::create();
    mBottomPanel->setBackGroundImage("textures/ui/bottom_panel.png");
    mBottomPanel->setContentSize(mBottomPanel->getBackGroundImageTextureSize());
    mBottomPanel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    item = ui::Button::create("textures/ui/btn_pause.png", "");
    item->setName("next_button");
    item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    item->setPosition(Vec2(200, mBottomPanel->getContentSize().height / 2.f));
    mBottomPanel->addChild(item);

    item = ui::Button::create("textures/ui/btn_ff.png", "");
    item->setName("menu_button");
    item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    item->setPosition(Vec2(mBottomPanel->getContentSize().width  - 200, mBottomPanel->getContentSize().height / 2.f));
    mBottomPanel->addChild(item);

    pLayer->addChild(mTopPanel);
    pLayer->addChild(mBottomPanel);
}
