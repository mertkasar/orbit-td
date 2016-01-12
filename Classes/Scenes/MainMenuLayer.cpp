#include "MainMenuLayer.h"
#include "World.h"

#include <ui/UILayout.h>
#include <ui/UIButton.h>
#include <SimpleAudioEngine.h>

USING_NS_CC;

MainMenuLayer::MainMenuLayer(World *world) {
    _world = world;
}

MainMenuLayer *MainMenuLayer::create(World *world) {
    MainMenuLayer *layer = new(std::nothrow) MainMenuLayer(world);

    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    } else {
        delete layer;
        layer = NULL;
        return NULL;
    }
}

bool MainMenuLayer::init() {
    if (!Layer::init())
        return false;

    _menu = ui::Layout::create();
    _menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _menu->setPosition(_world->_canvasCenter + Vec2(500.f, 0.f));

    auto button = ui::Button::create("textures/btn_start.png", "", "");
    button->setName("start_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(0.f, 150.f));
    button->addTouchEventListener(CC_CALLBACK_2(MainMenuLayer::startButtonCallback, this));
    _menu->addChild(button);

    button = ui::Button::create("textures/btn_scores.png", "", "");
    button->setName("highscores_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(0.f, 50.f));
    button->addTouchEventListener(CC_CALLBACK_2(MainMenuLayer::highscoresButtonCallback, this));
    _menu->addChild(button);

    button = ui::Button::create("textures/btn_options.png", "", "");
    button->setName("options_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(0.f, -50.f));
    button->addTouchEventListener(CC_CALLBACK_2(MainMenuLayer::optionsButtonCallback, this));
    _menu->addChild(button);

    button = ui::Button::create("textures/btn_quit.png", "", "");
    button->setName("quit_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(0.f, -150.f));
    button->addTouchEventListener(CC_CALLBACK_2(MainMenuLayer::quitButtonCallback, this));
    _menu->addChild(button);

    addChild(_menu);

    return true;
}

void MainMenuLayer::startButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        CCLOG("Start Button clicked!");
        _world->_audioEngine->playEffect("audio/click.wav");
    }
}

void MainMenuLayer::highscoresButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        CCLOG("Highscores Button clicked!");
        _world->_audioEngine->playEffect("audio/click.wav");
    }
}

void MainMenuLayer::optionsButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        CCLOG("Options Button clicked!");
        _world->_audioEngine->playEffect("audio/click.wav");
    }
}

void MainMenuLayer::quitButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        CCLOG("Quit Button clicked!");
        _world->_audioEngine->playEffect("audio/click.wav");
    }
}
