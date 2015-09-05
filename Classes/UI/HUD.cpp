#include <UI/HUD.h>

#include <2d/CCMenu.h>
#include <ui/UIImageView.h>
#include <ui/UIButton.h>
#include <ui/UIText.h>

#include <Scenes/GameScene.h>
#include <sstream>

USING_NS_CC;

void HUD::init(Layer *pLayer, GameScene *pGameScene) {
    mGameScene = pGameScene;

    mTopPanel = ui::Layout::create();
    mTopPanel->setBackGroundImage("textures/ui/top_panel.png");
    mTopPanel->setContentSize(mTopPanel->getBackGroundImageTextureSize());
    mTopPanel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    mTopPanel->setPosition(Vec2(0, 720));

    auto button = ui::Button::create("textures/ui/btn_next.png", "");
    button->setName("next_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(100, mTopPanel->getContentSize().height / 2.f));
    mTopPanel->addChild(button);

    button = ui::Button::create("textures/ui/btn_menu.png", "");
    button->setName("menu_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(mTopPanel->getContentSize().width - 100, mTopPanel->getContentSize().height / 2.f));
    mTopPanel->addChild(button);

    mBottomPanel = ui::Layout::create();
    mBottomPanel->setBackGroundImage("textures/ui/bottom_panel.png");
    mBottomPanel->setContentSize(mBottomPanel->getBackGroundImageTextureSize());
    mBottomPanel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    button = ui::Button::create("textures/ui/btn_pause.png", "");
    button->setName("next_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(200, mBottomPanel->getContentSize().height / 2.f));
    mBottomPanel->addChild(button);

    button = ui::Button::create("textures/ui/btn_ff.png", "");
    button->setName("menu_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(mBottomPanel->getContentSize().width - 200, mBottomPanel->getContentSize().height / 2.f));
    mBottomPanel->addChild(button);

    auto text = ui::Text::create("Total Coin:\t0", "fonts/ubuntu.ttf", 28);
    text->setName("#coin_text");
    text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    text->setPosition(
            Vec2(mBottomPanel->getContentSize().width / 2.f + 20, mBottomPanel->getContentSize().height / 2.f - 5));
    mBottomPanel->addChild(text);

    text = ui::Text::create("Total Life:\t10", "fonts/ubuntu.ttf", 28);
    text->setName("#life_text");
    text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    text->setPosition(
            Vec2(mBottomPanel->getContentSize().width / 2.f + 20, mBottomPanel->getContentSize().height / 2.f - 30));
    mBottomPanel->addChild(text);

    pLayer->addChild(mTopPanel);
    pLayer->addChild(mBottomPanel);
}

void HUD::update(float pDelta) {
    std::stringstream ss_c;
    ss_c << "Total Coin:\t" << mGameScene->getTotalCoin();

    auto text = static_cast<ui::Text *>(mBottomPanel->getChildByName("#coin_text"));
    text->setString(ss_c.str());

    std::stringstream ss_l;
    ss_l << "Remaining Life:\t" << mGameScene->getRemainingLife();

    text = static_cast<ui::Text *>(mBottomPanel->getChildByName("#life_text"));
    text->setString(ss_l.str());
}
