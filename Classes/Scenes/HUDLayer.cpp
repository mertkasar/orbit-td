#include <Scenes/HUDLayer.h>

#include <2d/CCMenu.h>
#include <2d/CCActionInterval.h>
#include <2d/CCActionInstant.h>
#include <ui/UIImageView.h>
#include <ui/UIButton.h>
#include <ui/UIText.h>
#include <SimpleAudioEngine.h>

#include <Scenes/World.h>
#include <Scenes/GameplayLayer.h>

#include <sstream>
#include <2d/CCSpriteFrameCache.h>

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

    this->setName("hud_layer");

    auto spriteCache = SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile("textures/ui_layer.plist");

    _topPanel = ui::Layout::create();
    _topPanel->setBackGroundImage("top_panel.png", ui::Widget::TextureResType::PLIST);
    _topPanel->setContentSize(_topPanel->getBackGroundImageTextureSize());
    _topPanel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _topPanel->setPosition(Vec2(0, 720));

    auto button = ui::Button::create("btn_next.png", "", "", ui::Widget::TextureResType::PLIST);
    button->setName("next_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(100, _topPanel->getContentSize().height / 2.f));
    button->addTouchEventListener(CC_CALLBACK_2(HUDLayer::nextButtonCallback, this));
    _topPanel->addChild(button);

    button = ui::Button::create("btn_menu.png", "", "", ui::Widget::TextureResType::PLIST);
    button->setName("menu_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(_topPanel->getContentSize().width - 100, _topPanel->getContentSize().height / 2.f));
    button->addTouchEventListener(CC_CALLBACK_2(HUDLayer::menuButtonCallback, this));
    _topPanel->addChild(button);

    _notificationPanel = ui::Layout::create();
    _notificationPanel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _notificationPanel->setPosition(Vec2(640, 680));

    _bottomPanel = ui::Layout::create();
    _bottomPanel->setBackGroundImage("bottom_panel.png", ui::Widget::TextureResType::PLIST);
    _bottomPanel->setContentSize(_bottomPanel->getBackGroundImageTextureSize());
    _bottomPanel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    button = ui::Button::create("btn_pause.png", "", "", ui::Widget::TextureResType::PLIST);
    button->setName("next_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(200, _bottomPanel->getContentSize().height / 2.f));
    button->addTouchEventListener(CC_CALLBACK_2(HUDLayer::pauseButtonCallback, this));
    _bottomPanel->addChild(button);

    button = ui::Button::create("btn_ff.png", "", "", ui::Widget::TextureResType::PLIST);
    button->setName("menu_button");
    button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    button->setPosition(Vec2(_bottomPanel->getContentSize().width - 200, _bottomPanel->getContentSize().height / 2.f));
    button->addTouchEventListener(CC_CALLBACK_2(HUDLayer::ffButtonCallback, this));
    _bottomPanel->addChild(button);

    auto text = ui::Text::create("Waves:0/0", "fonts/ubuntu.ttf", 28);
    text->setName("#wave_text");
    text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    text->setPosition(Vec2(220.f, 80.f));
    _topPanel->addChild(text);

    text = ui::Text::create("Total Coin:\t0", "fonts/ubuntu.ttf", 28);
    text->setName("#coin_text");
    text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    text->setPosition(
            Vec2(_bottomPanel->getContentSize().width / 2.f + 20, _bottomPanel->getContentSize().height / 2.f - 5));
    _bottomPanel->addChild(text);

    text = ui::Text::create("Total Life:\t10", "fonts/ubuntu.ttf", 28);
    text->setName("#life_text");
    text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    text->setPosition(
            Vec2(_bottomPanel->getContentSize().width / 2.f + 20, _bottomPanel->getContentSize().height / 2.f - 30));
    _bottomPanel->addChild(text);

    this->addChild(_topPanel);
    this->addChild(_notificationPanel);
    this->addChild(_bottomPanel);

    this->scheduleUpdate();

    return true;
}

void HUDLayer::update(float delta) {
    std::stringstream ss_c;
    ss_c << "Total Coin:\t" << _world->getTotalCoin();

    auto text = static_cast<ui::Text *>(_bottomPanel->getChildByName("#coin_text"));
    text->setString(ss_c.str());

    std::stringstream ss_l;
    ss_l << "Remaining Life:\t" << _world->getRemainingLife();

    text = static_cast<ui::Text *>(_bottomPanel->getChildByName("#life_text"));
    text->setString(ss_l.str());

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

void HUDLayer::ffButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {

        _world->_audioEngine->playEffect("audio/click.wav");
        notify('W', "Double time!");
    }
}
