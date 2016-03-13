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

    auto button = ui::Button::create("btn_play_n.png", "btn_play_t.png", "", ui::Widget::TextureResType::PLIST);
    button->setName("start_button");
    button->setPosition(Vec2(900.f, 250.f));
    button->addTouchEventListener(CC_CALLBACK_2(MainMenuLayer::startButtonCallback, this));
    addChild(button);

    if (_muted)
        button = ui::Button::create("btn_mute_n.png", "btn_mute_t.png", "", ui::Widget::TextureResType::PLIST);
    else
        button = ui::Button::create("btn_unmute_n.png", "btn_unmute_t.png", "", ui::Widget::TextureResType::PLIST);

    button->setName("unmute_button");
    button->setPosition(Vec2(60.f, 60.f));
    button->addTouchEventListener(CC_CALLBACK_2(MainMenuLayer::soundButtonCallback, this));
    addChild(button);

    button = ui::Button::create("btn_exit_n.png", "btn_exit_t.png", "", ui::Widget::TextureResType::PLIST);
    button->setName("exit_button");
    button->setPosition(Vec2(1220.f, 60.f));
    button->addTouchEventListener(CC_CALLBACK_2(MainMenuLayer::quitButtonCallback, this));
    addChild(button);

    setOpacity(0);

    return true;
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

void MainMenuLayer::soundButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        _muted = !_muted;
        _world->_prefs->setBoolForKey("muted", _muted);

        auto button = static_cast<ui::Button *>(sender);
        if (_muted) {
            button->loadTextures("btn_mute_n.png", "btn_mute_t.png", "", ui::Widget::TextureResType::PLIST);
            _world->_audioEngine->setBackgroundMusicVolume(0.f);
            _world->_audioEngine->setEffectsVolume(0.f);
        } else {
            _world->_audioEngine->setBackgroundMusicVolume(0.6f);
            _world->_audioEngine->setEffectsVolume(1.f);
            button->loadTextures("btn_unmute_n.png", "btn_unmute_t.png", "", ui::Widget::TextureResType::PLIST);
        }

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
