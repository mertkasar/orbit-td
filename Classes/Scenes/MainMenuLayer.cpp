#include <base/CCDirector.h>

#include "World.h"
#include "MainMenuLayer.h"
#include "../Entities/DialogBox.h"

#include <base/CCUserDefault.h>
#include <2d/CCSprite.h>
#include <2d/CCActionInstant.h>
#include <2d/CCActionInterval.h>
#include <SimpleAudioEngine.h>
#include <ui/UIButton.h>

USING_NS_CC;

MainMenuLayer::MainMenuLayer(World *world) {
    _world = world;
    CCLOG("MainMenuLayer created");
}

MainMenuLayer::~MainMenuLayer() {
    CCLOG("MainMenuLayer deleted");
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

    _muted = _world->_prefs->getBoolForKey("muted");

    setCascadeOpacityEnabled(true);

    auto button = ui::Button::create("textures/btn_play_n.png", "textures/btn_play_t.png", "");
    button->setName("start_button");
    button->setPosition(Vec2(900.f, 250.f));
    button->addTouchEventListener(CC_CALLBACK_2(MainMenuLayer::startButtonCallback, this));
    addChild(button);

    createOptionsMenu();

    button = ui::Button::create("textures/btn_help_n.png", "textures/btn_help_t.png", "");
    button->setName("help_button");
    button->setPosition(Vec2(180.f, 60.f));
    button->addTouchEventListener(CC_CALLBACK_2(MainMenuLayer::helpButtonCallback, this));
    addChild(button);

    button = ui::Button::create("textures/btn_exit_n.png", "textures/btn_exit_t.png", "");
    button->setName("exit_button");
    button->setPosition(Vec2(1220.f, 60.f));
    button->addTouchEventListener(CC_CALLBACK_2(MainMenuLayer::quitButtonCallback, this));
    addChild(button);

    setOpacity(0);

    return true;
}

void MainMenuLayer::createOptionsMenu() {
    auto frame = Sprite::create("textures/frame.png");
    frame->setPosition(60.f, 155.f);
    addChild(frame);

    auto button = ui::Button::create("textures/btn_options_n.png", "textures/btn_options_t.png", "");
    button->setName("options_button");
    button->setPosition(Vec2(60.f, 60.f));
    button->addTouchEventListener(CC_CALLBACK_2(MainMenuLayer::optionsButtonCallback, this));
    addChild(button);

    if (_muted)
        button = ui::Button::create("textures/btn_mute_n.png", "textures/btn_mute_t.png", "");
    else
        button = ui::Button::create("textures/btn_unmute_n.png", "textures/btn_unmute_t.png", "");

    button->setName("unmute_button");
    button->setPosition(Vec2(60.f, 155.f));
    button->addTouchEventListener(CC_CALLBACK_2(MainMenuLayer::soundButtonCallback, this));
    addChild(button);

    button = ui::Button::create("textures/btn_info_n.png", "textures/btn_info_t.png", "");
    button->setName("info_button");
    button->setPosition(Vec2(60.f, 245.f));
    addChild(button);
}

void MainMenuLayer::show(float delay) {
    auto showSequence = Sequence::create(DelayTime::create(delay),
                                         FadeIn::create(0.4f),
                                         NULL);
    runAction(showSequence);
}

void MainMenuLayer::hide(float delay) {
    auto hideSequence = Sequence::create(DelayTime::create(delay),
                                         FadeOut::create(0.4f),
                                         CallFunc::create([&]() { this->setVisible(false); }),
                                         NULL);
    runAction(hideSequence);
}

void MainMenuLayer::close(float delay) {
    hide();
    runAction(Sequence::create(DelayTime::create(delay + 0.4f), RemoveSelf::create(true), NULL));
}

void MainMenuLayer::startButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        auto button = static_cast<ui::Button *> (sender);
        button->setTouchEnabled(false);

        _world->setState(World::GAMEPLAY);
        _world->_audioEngine->playEffect("audio/click.wav");
    }
}

void MainMenuLayer::optionsButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        CCLOG("Options Button clicked!");
        _world->_audioEngine->playEffect("audio/click.wav");
    }
}

void MainMenuLayer::soundButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        _muted = !_muted;
        _world->_prefs->setBoolForKey("muted", _muted);

        auto button = static_cast<ui::Button *>(sender);
        if (_muted) {
            button->loadTextures("textures/btn_mute_n.png", "textures/btn_mute_t.png", "");
            _world->_audioEngine->setBackgroundMusicVolume(0.f);
            _world->_audioEngine->setEffectsVolume(0.f);
        } else {
            _world->_audioEngine->setBackgroundMusicVolume(0.6f);
            _world->_audioEngine->setEffectsVolume(1.f);
            button->loadTextures("textures/btn_unmute_n.png", "textures/btn_unmute_t.png", "");
        }

        _world->_audioEngine->playEffect("audio/click.wav");
    }
}

void MainMenuLayer::helpButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        CCLOG("Help Button clicked!");
        _world->_audioEngine->playEffect("audio/click.wav");
    }
}

void MainMenuLayer::quitButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        auto dialog = DialogBox::create(_world);
        dialog->setCaption("Are you sure you want to exit?");
        dialog->setAction([&](Ref *__sender, ui::Widget::TouchEventType __type) {
            if (__type == ui::Widget::TouchEventType::ENDED) {
                Director::getInstance()->end();
            }
        });
        dialog->runAction(dialog->show());
        _world->addChild(dialog);

        _world->_audioEngine->playEffect("audio/click.wav");
    }
}