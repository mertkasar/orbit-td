#include "HUDLayer.h"

#include "World.h"
#include "GameplayLayer.h"

#include <2d/CCMenu.h>
#include <2d/CCActionInterval.h>
#include <2d/CCActionInstant.h>
#include <2d/CCActionEase.h>
#include <2d/CCSprite.h>
#include <ui/UILayout.h>
#include <ui/UIImageView.h>
#include <ui/UIButton.h>
#include <ui/UIText.h>
#include <SimpleAudioEngine.h>

#include <sstream>
#include <Entities/DialogBox.h>
#include <Entities/CostIndicator.h>

USING_NS_CC;

HUDLayer::HUDLayer(World *world) {
    _world = world;
    CCLOG("HUDLayer created");
}

HUDLayer::~HUDLayer() {
    CCLOG("HUDLayer deleted");
}

HUDLayer *HUDLayer::create(World *world) {
    HUDLayer *layer = new(std::nothrow) HUDLayer(world);

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

    _topPanel = ui::Layout::create();
    _topPanel->setBackGroundImage("top_panel.png", ui::Widget::TextureResType::PLIST);
    _topPanel->setContentSize(_topPanel->getBackGroundImageTextureSize());
    _topPanel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _topPanel->setPosition(Vec2(0, 720));
    _topPanel->setOpacity(0);

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
    _bottomPanel->setOpacity(0);

    button = ui::Button::create("textures/btn_pause_n.png", "textures/btn_pause_t.png", "");
    button->setName("pause_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(1220.f, 60.f));
    button->addTouchEventListener(CC_CALLBACK_2(HUDLayer::pauseButtonCallback, this));
    _bottomPanel->addChild(button);

    auto text = ui::Text::create("Waves:0/0", "fonts/kenvector_future.ttf", 28);
    text->setName("#wave_text");
    text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    text->setPosition(Vec2(300.f, 90.f));
    _topPanel->addChild(text);

    _shieldBar = ui::Layout::create();
    _shieldBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _shieldBar->setPosition(Vec2(90.f, 10.f));

    auto sprite = Sprite::create("textures/hud_shield.png");
    sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _shieldBar->addChild(sprite);

    text = ui::Text::create("20", "fonts/kenvector_future.ttf", 32);
    text->setName("#shield_text");
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

    _energy = ui::Layout::create();
    _energy->setPosition(Vec2(700, 60.f));

    sprite = Sprite::create("textures/hud_energy.png");
    _energy->addChild(sprite);

    text = ui::Text::create("5000", "fonts/kenvector_future.ttf", 40);
    text->setName("#energy_text");
    text->setTextHorizontalAlignment(TextHAlignment::LEFT);
    text->setColor(Color::ICE);
    text->setPosition(Vec2(110.f, 0.f));
    _energy->addChild(text);

    _bottomPanel->addChild(_energy);

    addChild(_topPanel);
    addChild(_notificationPanel);
    addChild(_bottomPanel);

    return true;
}

void HUDLayer::update(float delta) {
    std::stringstream ss_c;
    ss_c << _world->getTotalCoin();

    auto text = static_cast<ui::Text *>(_energy->getChildByName("#energy_text"));
    text->setString(ss_c.str());
}

void HUDLayer::notify(char type, std::string message, float duration) {
    //Shift all existing notifications by one line
    for (auto notification : _notificationPanel->getChildren()) {
        notification->setPosition(notification->getPosition() - Vec2(0.f, 24.f));
    }

    //Create new notification
    auto notification = ui::Text::create(message, "fonts/kenvector_future.ttf", 24.f);
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
        std::stringstream ss;
        ss << life;

        auto text = static_cast<ui::Text *>(_shieldBar->getChildByName("#shield_text"));
        text->setString(ss.str());

        auto bar = static_cast<Sprite *>(_shieldBar->getChildByName("#shield_bar"));
        bar->setTextureRect(Rect(0, 0, life * 15, 35));
    }
}

void HUDLayer::updateWaveIndicators(int current, int total) {
    std::stringstream ss;
    ss << "Waves: " << current << " / " << total;

    auto text = static_cast<ui::Text *>(_topPanel->getChildByName("#wave_text"));
    text->setString(ss.str());
}

void HUDLayer::addCostIndicator(int cost, Vec2 position) {
    auto indicator = CostIndicator::create();
    indicator->update(cost);
    indicator->setPosition(position);

    auto riseAction = EaseExponentialOut::create(MoveBy::create(1.5f, Vec2(0.f, 20.f)));
    indicator->runAction(Sequence::create(riseAction, RemoveSelf::create(true), NULL));

    addChild(indicator);
}

void HUDLayer::show(float delay) { // Add interpolation animations
    _bottomPanel->runAction(Sequence::create(DelayTime::create(delay), FadeIn::create(0.3f), NULL));

    for (auto element : _topPanel->getChildren()) {
        auto elementPos = element->getPosition();
        element->setPosition(elementPos + Vec2(0.f, _topPanel->getContentSize().height));

        auto easedMoveTo = EaseBackOut::create(MoveTo::create(1.f, elementPos));
        element->runAction(Sequence::create(DelayTime::create(delay + 0.3f), easedMoveTo, NULL));
    }

    _topPanel->runAction(Sequence::create(DelayTime::create(delay), FadeIn::create(0.3f), NULL));

    for (auto element : _bottomPanel->getChildren()) {
        auto elementPos = element->getPosition();
        element->setPosition(elementPos - Vec2(0.f, _bottomPanel->getContentSize().height));

        auto easedMoveTo = EaseBackOut::create(MoveTo::create(1.f, elementPos));
        element->runAction(Sequence::create(DelayTime::create(delay + 0.3f), easedMoveTo, NULL));
    }

    scheduleUpdate();
}

void HUDLayer::hide(float delay) {
    _bottomPanel->runAction(Sequence::create(DelayTime::create(delay + 0.8f), FadeOut::create(0.3f), NULL));

    for (auto element : _topPanel->getChildren()) {
        auto desiredPos = Vec2(0.f, _topPanel->getContentSize().height);
        auto easedMoveTo = EaseBackIn::create(MoveBy::create(1.f, desiredPos));
        element->runAction(Sequence::create(DelayTime::create(delay), easedMoveTo, NULL));
    }

    _topPanel->runAction(Sequence::create(DelayTime::create(delay + 0.8f), FadeOut::create(0.3f), NULL));

    for (auto element : _bottomPanel->getChildren()) {
        auto desiredPos = -1 * Vec2(0.f, _topPanel->getContentSize().height);
        auto easedMoveTo = EaseBackIn::create(MoveBy::create(1.f, desiredPos));
        element->runAction(Sequence::create(DelayTime::create(delay), easedMoveTo, NULL));
    }

    unscheduleUpdate();
}

void HUDLayer::close(float delay) {
    hide();
    runAction(Sequence::create(DelayTime::create(delay + 1.1f),
                               RemoveSelf::create(true),
                               NULL));
}

void HUDLayer::menuButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        _dialogBox = DialogBox::create(_world);
        _dialogBox->setCaption("Are you sure you want to\n   back to the main menu?");
        _dialogBox->setAction([&](Ref *__sender, ui::Widget::TouchEventType __type) {
            if (__type == ui::Widget::TouchEventType::ENDED) {
                _dialogBox->runAction(Sequence::create(_dialogBox->hide(), RemoveSelf::create(true), NULL));

                _world->setState(World::MAIN_MENU);
                _world->_audioEngine->playEffect("audio/click.wav");
            }
        });
        _dialogBox->runAction(_dialogBox->show());

        _world->addChild(_dialogBox);
        _world->_audioEngine->playEffect("audio/click.wav");
    }
}

void HUDLayer::nextButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        if (!_world->isCleared()) {
            _world->spawnNextWave(0.f);
            notify('W', "Coming next wave!");
        } else
            notify('E', "No waves remained!");
        _world->_audioEngine->playEffect("audio/click.wav");
    }
}

void HUDLayer::pauseButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        auto layer = _world->_gameplayLayer;
        auto button = static_cast<ui::Button *>(sender);

        if (layer->isPaused()) {
            layer->resumeScene();
            button->loadTextures("textures/btn_pause_n.png", "textures/btn_pause_t.png", "");
            notify('W', "Game resumed!");
        } else {
            layer->pauseScene();
            button->loadTextures("textures/btn_resume_n.png", "textures/btn_resume_t.png", "");
            notify('W', "Game paused!");
        }

        _world->_audioEngine->playEffect("audio/click.wav");
    }
}
