#include "ResultPanel.h"

#include "../Scenes/World.h"

#include <base/CCUserDefault.h>
#include <base/CCEventListenerTouch.h>
#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <2d/CCActionInterval.h>
#include <2d/CCActionInstant.h>
#include <2d/CCSprite.h>
#include <ui/UIText.h>
#include <ui/UIButton.h>
#include <SimpleAudioEngine.h>
#include <sstream>

USING_NS_CC;

ResultPanel::ResultPanel(World *world) {
    _world = world;
    CCLOG("ResultPanel created");
}

ResultPanel::~ResultPanel() {
    CCLOG("ResultPanel deleted");
}

ResultPanel *ResultPanel::create(World *world, bool win, int point, int energy, int shield) {
    ResultPanel *layer = new(std::nothrow) ResultPanel(world);

    if (layer && layer->init(win, point, energy, shield)) {
        layer->autorelease();
        return layer;
    } else {
        delete layer;
        layer = NULL;
        return NULL;
    }
}

bool ResultPanel::init(bool win, int point, int energy, int shield) {
    if (!ui::Layout::init())
        return false;

    _score = point + energy + shield * 100;
    _scoreCounter = 0;
    _incrementVal = _score / 60;

    auto contentSize = Size(480, 760);
    auto center = Vec2(contentSize / 2.f);

    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setBackGroundImage("textures/bg_panel.png");
    setBackGroundImageScale9Enabled(true);
    setContentSize(contentSize);
    setPosition(_world->_canvasCenter);
    setScaleX(0.f);

    auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch *touch, Event *event) { return true; };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    _titleText = ui::Text::create("", "fonts/kenvector_future.ttf", 48);
    _titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _titleText->setPosition(Vec2(225.f, 500.f));
    _titleText->setVisible(false);
    addChild(_titleText);

    _scoreText = ui::Text::create("", "fonts/kenvector_future.ttf", 48);
    _scoreText->setPosition(Vec2(225.f, 400.f));
    _scoreText->setColor(Color::ORANGE);
    _scoreText->setVisible(false);
    addChild(_scoreText);

    auto button = ui::Button::create("textures/btn_replay_n.png", "textures/btn_replay_t.png", "");
    button->setPosition(center + Vec2(-110.f, -100.f));
    button->addTouchEventListener(CC_CALLBACK_2(ResultPanel::replayButtonCallback, this));
    button->setVisible(false);
    addChild(button);

    button = ui::Button::create("textures/btn_main_menu_n.png", "textures/btn_main_menu_t.png", "");
    button->setPosition(center + Vec2(110.f, -100.f));
    button->addTouchEventListener(CC_CALLBACK_2(ResultPanel::menuButtonCallback, this));
    button->setVisible(false);
    addChild(button);

    std::string title;
    Color3B color;

    if (win) {
        title = "CLEARED!";
        color = Color::GREEN;
    } else {
        title = "FAILED!";
        color = Color::RED;
    }

    _titleText->setString(title);
    _titleText->setColor(color);

    int lastScore = _world->_prefs->getIntegerForKey("highscore");
    bool highScore = _score > lastScore;
    if (highScore) {
        _world->_prefs->setIntegerForKey("highscore", _score);

        auto highScoreText = ui::Text::create("New Highscore!", "fonts/kenvector_future.ttf", 24);
        highScoreText->setColor(Color::GREEN);
        highScoreText->setOpacity(0);
        highScoreText->setPosition(_scoreText->getPosition() - Vec2(0.f, 50.f));
        highScoreText->runAction(Sequence::create(DelayTime::create(1.5f), FadeIn::create(0.3f), NULL));
        addChild(highScoreText);
    }

    scheduleUpdate();

    return true;
}

void ResultPanel::update(float delta) {
    //TODO: Not frame rate independent!
    if (_score > 0) {
        _scoreCounter = _scoreCounter + _incrementVal;

        std::stringstream scoreSS;
        scoreSS << _scoreCounter;

        _scoreText->setString(scoreSS.str());

        _score = _score - _incrementVal;
    } else {
        _scoreCounter = _scoreCounter + _score;

        std::stringstream scoreSS;
        scoreSS << _scoreCounter;

        _scoreText->setString(scoreSS.str());

        unscheduleUpdate();
    }
}

FiniteTimeAction *ResultPanel::show() {
    auto showContent = [&]() {
        for (auto child : getChildren())
            child->setVisible(true);
    };

    return Sequence::create(ScaleTo::create(0.2f, 1.f), CallFunc::create(showContent), NULL);
}

FiniteTimeAction *ResultPanel::hide() {
    auto hideContent = [&]() {
        for (auto child : getChildren())
            child->setVisible(false);
    };

    return Sequence::create(CallFunc::create(hideContent), ScaleTo::create(0.2f, 1.f, 0.f), NULL);
}

void ResultPanel::replayButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        runAction(Sequence::create(hide(), RemoveSelf::create(true), NULL));

        _world->resetGame();
        _world->_audioEngine->playEffect("audio/click.wav");
    }
}

void ResultPanel::menuButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        runAction(Sequence::create(hide(), RemoveSelf::create(true), NULL));

        _world->setState(World::MAIN_MENU);
        _world->_audioEngine->playEffect("audio/click.wav");
    }
}
