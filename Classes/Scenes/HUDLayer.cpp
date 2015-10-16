#include <Scenes/HUDLayer.h>

#include <2d/CCMenu.h>
#include <2d/CCActionInterval.h>
#include <2d/CCActionInstant.h>
#include <ui/UIImageView.h>
#include <ui/UIButton.h>
#include <ui/UIText.h>

#include "World.h"

#include <sstream>

USING_NS_CC;

HUDLayer::HUDLayer(World *pWorld) {
    mWorld = pWorld;
}

HUDLayer *HUDLayer::create(World *pWorld) {
    HUDLayer *layer = new(std::nothrow) HUDLayer(pWorld);

    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    } else {
        delete layer;
        layer = NULL;
        return NULL;
    }
}

bool HUDLayer::init() {
    if (!Layer::init())
        return false;

    this->setName("hud_layer");

    mTopPanel = ui::Layout::create();
    mTopPanel->setBackGroundImage("textures/ui/top_panel.png");
    mTopPanel->setContentSize(mTopPanel->getBackGroundImageTextureSize());
    mTopPanel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    mTopPanel->setPosition(Vec2(0, 720));

    auto button = ui::Button::create("textures/ui/btn_next.png", "");
    button->setName("next_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(100, mTopPanel->getContentSize().height / 2.f));
    button->addTouchEventListener(CC_CALLBACK_2(HUDLayer::nextButtonCallback, this));
    mTopPanel->addChild(button);

    button = ui::Button::create("textures/ui/btn_menu.png", "");
    button->setName("menu_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(mTopPanel->getContentSize().width - 100, mTopPanel->getContentSize().height / 2.f));
    button->addTouchEventListener(CC_CALLBACK_2(HUDLayer::menuButtonCallback, this));
    mTopPanel->addChild(button);

    mNotificationPanel = ui::Layout::create();
    mNotificationPanel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    mNotificationPanel->setPosition(Vec2(640, 680));

    mBottomPanel = ui::Layout::create();
    mBottomPanel->setBackGroundImage("textures/ui/bottom_panel.png");
    mBottomPanel->setContentSize(mBottomPanel->getBackGroundImageTextureSize());
    mBottomPanel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    button = ui::Button::create("textures/ui/btn_pause.png", "");
    button->setName("next_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(200, mBottomPanel->getContentSize().height / 2.f));
    button->addTouchEventListener(CC_CALLBACK_2(HUDLayer::pauseButtonCallback, this));
    mBottomPanel->addChild(button);

    button = ui::Button::create("textures/ui/btn_ff.png", "");
    button->setName("menu_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(mBottomPanel->getContentSize().width - 200, mBottomPanel->getContentSize().height / 2.f));
    button->addTouchEventListener(CC_CALLBACK_2(HUDLayer::ffButtonCallback, this));
    mBottomPanel->addChild(button);

    auto text = ui::Text::create("Waves:0/0", "fonts/ubuntu.ttf", 28);
    text->setName("#wave_text");
    text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    text->setPosition(Vec2(220.f, 80.f));
    mTopPanel->addChild(text);

    text = ui::Text::create("Total Coin:\t0", "fonts/ubuntu.ttf", 28);
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

    this->addChild(mTopPanel);
    this->addChild(mNotificationPanel);
    this->addChild(mBottomPanel);

    this->scheduleUpdate();

    return true;
}

void HUDLayer::update(float pDelta) {
    std::stringstream ss_c;
    ss_c << "Total Coin:\t" << mWorld->getTotalCoin();

    auto text = static_cast<ui::Text *>(mBottomPanel->getChildByName("#coin_text"));
    text->setString(ss_c.str());

    std::stringstream ss_l;
    ss_l << "Remaining Life:\t" << mWorld->getRemainingLife();

    text = static_cast<ui::Text *>(mBottomPanel->getChildByName("#life_text"));
    text->setString(ss_l.str());

    std::stringstream ss_w;
    ss_w << "Waves: " << 0 << " / " << 0;

    text = static_cast<ui::Text *>(mTopPanel->getChildByName("#wave_text"));
    text->setString(ss_w.str());
}

void HUDLayer::notify(char pType, std::string pMessage, float pDuration) {
    //Shift all existing notifications by one line
    for (auto notification : mNotificationPanel->getChildren()) {
        notification->setPosition(notification->getPosition() - Vec2(0.f, 24.f));
    }

    //Create new notification
    auto notification = ui::Text::create(pMessage, "fonts/ubuntu.ttf", 24.f);
    notification->setTextHorizontalAlignment(TextHAlignment::CENTER);
    notification->runAction(Sequence::create(FadeOut::create(pDuration), RemoveSelf::create(), NULL));

    switch (pType) {
        case 'I':
            notification->setColor(Color3B(46, 204, 113));
            break;
        case 'W':
            notification->setColor(Color3B(241, 196, 15));
            break;
        case 'E':
            notification->setColor(Color3B(231, 76, 60));
            break;
        default:
            break;
    }

    mNotificationPanel->addChild(notification);
}

void HUDLayer::menuButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        notify('W', "Opening menu!");
    }
}

void HUDLayer::nextButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        if (mWorld->spawnNextWave())
            notify('W', "Coming next wave!");
        else
            notify('E', "Out of waves!");
    }
}

void HUDLayer::pauseButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        notify('W', "Game paused!");
    }
}

void HUDLayer::ffButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        notify('W', "Double time!");
    }
}
