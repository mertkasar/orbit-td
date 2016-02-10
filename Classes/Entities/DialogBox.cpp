#include "DialogBox.h"

#include "../Scenes/World.h"

#include <ui/UIText.h>
#include <ui/UIButton.h>
#include <2d/CCActionInstant.h>
#include <2d/CCActionInterval.h>
#include <SimpleAudioEngine.h>

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

    auto contentSize = Size(620, 250);
    auto center = Vec2(contentSize / 2.f);

    setCascadeOpacityEnabled(true);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setBackGroundImage("textures/bg_panel.png");
    setBackGroundImageScale9Enabled(true);
    setContentSize(contentSize);
    setPosition(_world->_canvasCenter);
    setScaleY(0.f);

    _text = ui::Text::create();
    _text->setFontName("fonts/kenvector_future.ttf");
    _text->setFontSize(24);
    _text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _text->setColor(Color::ICE);
    _text->setPosition(center + Vec2(0.f, 50.f));
    _text->setVisible(false);
    addChild(_text);

    auto button = ui::Button::create("textures/btn_no_n.png", "textures/btn_no_t.png", "");
    button->setPosition(center + Vec2(-150.f, -50.f));
    button->addTouchEventListener(CC_CALLBACK_2(DialogBox::noButtonCallback, this));
    button->setVisible(false);
    addChild(button);

    _yesButton = ui::Button::create("textures/btn_yes_n.png", "textures/btn_yes_t.png", "");
    _yesButton->setPosition(center + Vec2(150.f, -50.f));
    _yesButton->setVisible(false);
    addChild(_yesButton);

    return true;
}

void DialogBox::setCaption(std::string caption) {
    _text->setString(caption);
}

void DialogBox::setAction(const ccWidgetTouchCallback& action){
    _yesButton->addTouchEventListener(action);
}

FiniteTimeAction *DialogBox::show() {
    auto showContent = [&](){
        for (auto child : getChildren())
            child->setVisible(true);
    };

    return Sequence::create(ScaleTo::create(0.2f, 1.f), CallFunc::create(showContent), NULL);
}

FiniteTimeAction *DialogBox::hide() {
    auto hideContent = [&](){
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
