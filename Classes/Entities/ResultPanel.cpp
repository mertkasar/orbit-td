#include "ResultPanel.h"

#include "../Scenes/World.h"

#include <SimpleAudioEngine.h>
#include <2d/CCActionInterval.h>
#include <2d/CCActionInstant.h>
#include <2d/CCSprite.h>
#include <ui/UIText.h>
#include <ui/UIButton.h>

USING_NS_CC;

ResultPanel::ResultPanel(World *world) {
    _world = world;
}

ResultPanel *ResultPanel::create(World *world) {
    ResultPanel *layer = new(std::nothrow) ResultPanel(world);

    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    } else {
        delete layer;
        layer = NULL;
        return NULL;
    }
}

bool ResultPanel::init() {
    if (!ui::Layout::init())
        return false;

    auto contentSize = Size(480, 760);
    auto center = Vec2(contentSize / 2.f);

    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setBackGroundImage("textures/bg_panel.png");
    setBackGroundImageScale9Enabled(true);
    setContentSize(contentSize);
    setPosition(_world->_canvasCenter);
    setScaleX(0.f);

    _header = ui::Text::create("CLEARED!", "fonts/kenvector_future.ttf", 48);
    _header->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _header->setColor(Color::GREEN);
    _header->setPosition(Vec2(225.f, 600.f));
    _header->setVisible(false);
    addChild(_header);

    auto scoreLayout = ui::Layout::create();
    scoreLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    scoreLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    scoreLayout->setPosition(Vec2(50.f, 360.f));
    scoreLayout->setVisible(false);

    auto text = ui::Text::create("Point: 123", "fonts/kenvector_future.ttf", 24);
    scoreLayout->addChild(text);

    text = ui::Text::create("Gold: 12", "fonts/kenvector_future.ttf", 24);
    scoreLayout->addChild(text);

    text = ui::Text::create("Shield: 1", "fonts/kenvector_future.ttf", 24);
    scoreLayout->addChild(text);

    addChild(scoreLayout);

    auto button = ui::Button::create("textures/btn_replay_n.png", "textures/btn_replay_t.png", "");
    button->setPosition(center + Vec2(-110.f, -200.f));
    button->addTouchEventListener(CC_CALLBACK_2(ResultPanel::replayButtonCallback, this));
    button->setVisible(false);
    addChild(button);

    button = ui::Button::create("textures/btn_main_menu_n.png", "textures/btn_main_menu_t.png", "");
    button->setPosition(center + Vec2(110.f, -200.f));
    button->addTouchEventListener(CC_CALLBACK_2(ResultPanel::menuButtonCallback, this));
    button->setVisible(false);
    addChild(button);

    return true;
}

FiniteTimeAction *ResultPanel::show() {
    auto showContent = [&](){
        for (auto child : getChildren())
            child->setVisible(true);
    };

    return Sequence::create(ScaleTo::create(0.2f, 1.f), CallFunc::create(showContent), NULL);
}

FiniteTimeAction *ResultPanel::hide() {
    auto hideContent = [&](){
        for (auto child : getChildren())
            child->setVisible(false);
    };

    return Sequence::create(CallFunc::create(hideContent), ScaleTo::create(0.2f, 1.f, 0.f), NULL);
}

void ResultPanel::replayButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        runAction(hide());

        _world->resetGame();
        _world->_audioEngine->playEffect("audio/click.wav");
    }
}

void ResultPanel::menuButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        runAction(hide());

        _world->setState(World::MAIN_MENU);
        _world->_audioEngine->playEffect("audio/click.wav");
    }
}
