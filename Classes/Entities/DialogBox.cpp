#include "DialogBox.h"

#include "../Scenes/World.h"

#include <ui/UIText.h>
#include <ui/UIButton.h>
#include <2d/CCActionInstant.h>
#include <2d/CCActionInterval.h>
#include <SimpleAudioEngine.h>
#include <base/CCEventListenerTouch.h>
#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>

USING_NS_CC;

DialogBox::DialogBox(World *world) {
    _world = world;
    CCLOG("DialogBox created");
}

DialogBox::~DialogBox() {
    CCLOG("DialogBox deleted");
}

DialogBox *DialogBox::create(World *world) {
    DialogBox *ret = new(std::nothrow) DialogBox(world);

    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool DialogBox::init() {
    if (!ui::Layout::init())
        return false;

    const Size &contentSize = Size(620, 250);
    auto center = Vec2(contentSize / 2.f);

    setContentSize(contentSize);
    setCascadeOpacityEnabled(true);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setBackGroundImage("bg_panel.png", TextureResType::PLIST);
    setBackGroundImageScale9Enabled(true);
    setPosition(_world->_canvasCenter);
    setScaleY(0.f);

    auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch *touch, Event *event) { return true; };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    _text = ui::Text::create();
    _text->setFontName("fonts/kenvector_future.ttf");
    _text->setFontSize(24);
    _text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _text->setColor(Palette::ICE);
    _text->setPosition(center + Vec2(0.f, 50.f));
    _text->setVisible(false);
    addChild(_text);

    auto button = ui::Button::create("btn_no_n.png", "btn_no_t.png", "", TextureResType::PLIST);
    button->setPosition(center + Vec2(-150.f, -50.f));
    button->addTouchEventListener(CC_CALLBACK_2(DialogBox::noButtonCallback, this));
    button->setVisible(false);
    addChild(button);

    _yesButton = ui::Button::create("btn_yes_n.png", "btn_yes_t.png", "", TextureResType::PLIST);
    _yesButton->setPosition(center + Vec2(150.f, -50.f));
    _yesButton->setVisible(false);
    addChild(_yesButton);

    return true;
}

void DialogBox::setCaption(std::string caption) {
    _text->setString(caption);
}

void DialogBox::setAction(const ccWidgetTouchCallback &action) {
    _yesButton->addTouchEventListener(action);
}

FiniteTimeAction *DialogBox::show() {
    auto showContent = [&]() {
        for (auto child : getChildren())
            child->setVisible(true);
    };

    return Sequence::create(ScaleTo::create(0.2f, 1.f), CallFunc::create(showContent), NULL);
}

FiniteTimeAction *DialogBox::hide() {
    auto hideContent = [&]() {
        for (auto child : getChildren())
            child->setVisible(false);
    };

    return Sequence::create(CallFunc::create(hideContent), ScaleTo::create(0.2f, 1.f, 0.f), NULL);
}

void DialogBox::noButtonCallback(Ref *sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        runAction(Sequence::create(hide(), RemoveSelf::create(true), NULL));

        _world->_audioEngine->playEffect("audio/click.wav");
    }
}
