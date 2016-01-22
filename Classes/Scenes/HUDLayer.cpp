#include "HUDLayer.h"

#include "World.h"
#include "GameplayLayer.h"

#include <2d/CCMenu.h>
#include <2d/CCActionInterval.h>
#include <2d/CCActionInstant.h>
#include <2d/CCActionEase.h>
#include <ui/UILayout.h>
#include <ui/UIImageView.h>
#include <ui/UIButton.h>
#include <ui/UIText.h>
#include <SimpleAudioEngine.h>

#include <sstream>
#include <2d/CCSprite.h>

USING_NS_CC;

HUDLayer *HUDLayer::create(World *world) {
    HUDLayer *layer = new(std::nothrow) HUDLayer();

    if (layer && layer->init(world)) {
        layer->autorelease();
        return layer;
    } else {
        delete layer;
        layer = NULL;
        return NULL;
    }
}

bool HUDLayer::init(World *world) {
    if (!Layer::init())
        return false;

    _world = world;

    _topPanel = ui::Layout::create();
    _topPanel->setBackGroundImage("top_panel.png", ui::Widget::TextureResType::PLIST);
    _topPanel->setContentSize(_topPanel->getBackGroundImageTextureSize());
    _topPanel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _topPanel->setPosition(Vec2(0, 720));

    auto button = ui::Button::create("textures/btn_next_n.png", "textures/btn_next_t.png", "");
    button->setName("next_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(140.f, 90.f));
    button->addTouchEventListener(CC_CALLBACK_2(HUDLayer::nextButtonCallback, this));
    _topPanel->addChild(button);

    button = ui::Button::create("textures/btn_menu_n.png", "textures/btn_menu_t.png", "");
    button->setName("menu_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(1220.f, 90.f));
    button->addTouchEventListener(CC_CALLBACK_2(HUDLayer::menuButtonCallback, this));
    _topPanel->addChild(button);

    _notificationPanel = ui::Layout::create();
    _notificationPanel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _notificationPanel->setPosition(Vec2(640, 680));

    _bottomPanel = ui::Layout::create();
    _bottomPanel->setBackGroundImage("bottom_panel.png", ui::Widget::TextureResType::PLIST);
    _bottomPanel->setContentSize(_bottomPanel->getBackGroundImageTextureSize());
    _bottomPanel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    button = ui::Button::create("textures/btn_pause_n.png", "textures/btn_pause_t.png", "");
    button->setName("pause_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(1220.f, 60.f));
    button->addTouchEventListener(CC_CALLBACK_2(HUDLayer::pauseButtonCallback, this));
    _bottomPanel->addChild(button);

    auto text = ui::Text::create("Waves:0/0", "fonts/ubuntu.ttf", 28);
    text->setName("#wave_text");
    text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    text->setPosition(Vec2(260.f, 90.f));
    _topPanel->addChild(text);

    text = ui::Text::create("Total Coin:\t0", "fonts/ubuntu.ttf", 28);
    text->setName("#coin_text");
    text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    text->setPosition(
            Vec2(_bottomPanel->getContentSize().width / 2.f + 20, _bottomPanel->getContentSize().height / 2.f - 5));
    _bottomPanel->addChild(text);

    _shieldBar = ui::Layout::create();
    _shieldBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _shieldBar->setPosition(Vec2(90.f, 10.f));

    auto sprite = Sprite::create("textures/hud_shield.png");
    sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _shieldBar->addChild(sprite);

    text = ui::Text::create("20", "fonts/kenvector_future.ttf", 32);
    text->setName("#life_text");
    text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    text->setColor(Color::ORANGE);
    text->setPosition(Vec2(50.f, 50.f));
    _shieldBar->addChild(text);

    sprite = Sprite::create("textures/shield_bar.png");
    sprite->setName("#shield_bar");
    sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    sprite->setPosition(108.f, 35.f);
    _shieldBar->addChild(sprite);

    _bottomPanel->addChild(_shieldBar);

    // Add interpolation animations
    float d = 3.f;

    _bottomPanel->setOpacity((GLubyte) 0.f);
    _bottomPanel->runAction(Sequence::create(DelayTime::create(d), FadeIn::create(0.3f), NULL));

    for (auto element : _topPanel->getChildren()) {
        auto elementPos = element->getPosition();
        element->setPosition(elementPos + Vec2(0.f, _topPanel->getContentSize().height));

        auto easedMoveTo = EaseBackOut::create(MoveTo::create(1.f, elementPos));
        element->runAction(Sequence::create(DelayTime::create(d + 0.3f), easedMoveTo, NULL));
    }

    _topPanel->setOpacity((GLubyte) 0.f);
    _topPanel->runAction(Sequence::create(DelayTime::create(d), FadeIn::create(0.3f), NULL));

    for (auto element : _bottomPanel->getChildren()) {
        auto elementPos = element->getPosition();
        element->setPosition(elementPos - Vec2(0.f, _bottomPanel->getContentSize().height));

        auto easedMoveTo = EaseBackOut::create(MoveTo::create(1.f, elementPos));
        element->runAction(Sequence::create(DelayTime::create(d + 0.3f), easedMoveTo, NULL));
    }

    addChild(_topPanel);
    addChild(_notificationPanel);
    addChild(_bottomPanel);

    scheduleUpdate();

    return true;
}

void HUDLayer::update(float delta) {
    std::stringstream ss_c;
    ss_c << "Total Coin:\t" << _world->getTotalCoin();

    auto text = static_cast<ui::Text *>(_bottomPanel->getChildByName("#coin_text"));
    text->setString(ss_c.str());

    std::stringstream ss_w;
    ss_w << "Waves: " << 0 << " / " << 0;

    text = static_cast<ui::Text *>(_topPanel->getChildByName("#wave_text"));
    text->setString(ss_w.str());
}

void HUDLayer::notify(char type, std::string message, float duration) {
    //Shift all existing notifications by one line
    for (auto notification : _notificationPanel->getChildren()) {
        notification->setPosition(notification->getPosition() - Vec2(0.f, 24.f));
    }

    //Create new notification
    auto notification = ui::Text::create(message, "fonts/ubuntu.ttf", 24.f);
    notification->setTextHorizontalAlignment(TextHAlignment::CENTER);
    notification->runAction(Sequence::create(FadeOut::create(duration), RemoveSelf::create(), NULL));

    switch (type) {
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

    _notificationPanel->addChild(notification);
}

void HUDLayer::updateLife() {
    unsigned int life = _world->getRemainingLife();

    if (life <= 20) {
        std::stringstream ss_l;
        ss_l << life;

        auto text = static_cast<ui::Text *>(_shieldBar->getChildByName("#life_text"));
        text->setString(ss_l.str());

        auto bar = static_cast<Sprite *>(_shieldBar->getChildByName("#shield_bar"));
        bar->setTextureRect(Rect(0, 0, life * 15, 35));
    }
}


void HUDLayer::menuButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        _world->_audioEngine->playEffect("audio/click.wav");
        notify('W', "Opening menu!");
    }
}

void HUDLayer::nextButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        _world->_audioEngine->playEffect("audio/click.wav");

        if (_world->spawnNextWave())
            notify('W', "Coming next wave!");
        else
            notify('E', "Out of waves!");
    }
}

void HUDLayer::pauseButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        auto layer = _world->_gameplayLayer;

        if (layer->isPaused())
            layer->resumeScene();
        else
            layer->pauseScene();

        _world->_audioEngine->playEffect("audio/click.wav");
        notify('W', "Game paused!");
    }
}
