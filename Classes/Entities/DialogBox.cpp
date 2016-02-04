#include <ui/UIText.h>
#include <Scenes/World.h>
#include <ui/UIButton.h>
#include <2d/CCActionInstant.h>
#include "DialogBox.h"

USING_NS_CC;

DialogBox *DialogBox::create(World *world) {
    DialogBox *ret = new(std::nothrow) DialogBox();

    if (ret && ret->init(world)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool DialogBox::init(World *world) {
    if (!ui::Layout::init())
        return false;

    _world = world;

    setTouchEnabled(true);
    setSwallowTouches(true);
    setCascadeOpacityEnabled(true);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setBackGroundImage("textures/dialog_bg.png");
    setPosition(_world->_canvasCenter);

    _text = ui::Text::create();
    _text->setFontName("fonts/kenvector_future.ttf");
    _text->setFontSize(24);
    _text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _text->setColor(Color::ICE);
    _text->setPosition(Vec2(0.f, 50.f));
    addChild(_text);

    auto button = ui::Button::create("textures/btn_no_n.png", "textures/btn_no_t.png", "");
    button->setPosition(Vec2(-150.f, -50.f));
    button->addTouchEventListener(CC_CALLBACK_2(DialogBox::noButtonCallback, this));
    addChild(button);

    _yesButton = ui::Button::create("textures/btn_yes_n.png", "textures/btn_yes_t.png", "");
    _yesButton->setPosition(Vec2(150.f, -50.f));
    addChild(_yesButton);

    return true;
}

void DialogBox::setCaption(std::string caption) {
    _text->setString(caption);
}

void DialogBox::setAction(const ccWidgetTouchCallback& action){
    _yesButton->addTouchEventListener(action);
}

void DialogBox::noButtonCallback(Ref *sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        runAction(RemoveSelf::create(true));
    }
}
