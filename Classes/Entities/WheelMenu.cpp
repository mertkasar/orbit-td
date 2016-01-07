#include <Entities/WheelMenu.h>

#include <2d/CCMenu.h>
#include <2d/CCActionInterval.h>
#include <ui/UIImageView.h>
#include <ui/UIButton.h>
#include <SimpleAudioEngine.h>

#include <Scenes/World.h>
#include <Scenes/MapLayer.h>
#include <Scenes/GameplayLayer.h>
#include <Entities/Turret.h>
#include <2d/CCSprite.h>
#include <Entities/TowerMock.h>

USING_NS_CC;

WheelMenu *WheelMenu::create(World *world) {
    WheelMenu *ret = new(std::nothrow) WheelMenu();

    if (ret && ret->init(world)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool WheelMenu::init(World *world) {
    if (!Node::init())
        return false;

    _world = world;

    _currentTile = Vec2(-1, -1);
    _lastCoin = 0;

    _rootMenu = Node::create();
    _rootMenu->setContentSize(Size(250.f, 250.f));

    _purchaseMenu = ui::Layout::create();

    const float SHIFT = 90.f;

    auto item = ui::Button::create("btn_trt.png", "", "", ui::Widget::TextureResType::PLIST);
    item->setTag(MACHINE_GUN);
    item->setPosition(Vec2(-SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    _purchaseMenu->addChild(item);

    item = ui::Button::create("btn_lsr.png", "", "", ui::Widget::TextureResType::PLIST);
    item->setTag(LASER_GUN);
    item->setPosition(Vec2(0, SHIFT));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    _purchaseMenu->addChild(item);

    item = ui::Button::create("btn_rl.png", "", "", ui::Widget::TextureResType::PLIST);
    item->setTag(MISSILE_LAUNCHER);
    item->setPosition(Vec2(SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    _purchaseMenu->addChild(item);

    _verboseMenu = ui::Layout::create();

    item = ui::Button::create("btn_upgrade.png", "", "", ui::Widget::TextureResType::PLIST);
    item->setTag(UPGRADE);
    item->setPosition(Vec2(-SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::upgradeButtonCallback, this));
    _verboseMenu->addChild(item);

    item = ui::Button::create("btn_sell.png", "", "", ui::Widget::TextureResType::PLIST);
    item->setTag(SELL);
    item->setPosition(Vec2(SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::sellButtonCallback, this));
    _verboseMenu->addChild(item);

    _validationMenu = ui::Layout::create();

    item = ui::Button::create("btn_accept.png", "", "", ui::Widget::TextureResType::PLIST);
    item->setTag(ACCEPT);
    item->setPosition(Vec2(SHIFT, 0.f));
    _validationMenu->addChild(item);

    item = ui::Button::create("btn_decline.png", "", "", ui::Widget::TextureResType::PLIST);
    item->setTag(DECLINE);
    item->setPosition(Vec2(-SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::declineButtonCallback, this));
    _validationMenu->addChild(item);

    _mock = TowerMock::create();

    _rootMenu->addChild(_purchaseMenu);
    _rootMenu->addChild(_verboseMenu);
    _rootMenu->addChild(_validationMenu);

    addChild(_mock);
    addChild(_rootMenu);

    setState(IDLE);

    scheduleUpdate();

    return true;
}

void WheelMenu::update(float delta) {
    auto totalCoin = _world->getTotalCoin();

    if (totalCoin != _lastCoin) {
        _lastCoin = totalCoin;

        updateButtonStates();
    }
}

void WheelMenu::setState(WheelMenu::State state) {
    for (auto menu :  _rootMenu->getChildren())
        menu->setVisible(false);

    Node *menu = nullptr;

    switch (state) {
        case IDLE:
            break;
        case PURCHASE:
            _purchaseMenu->setVisible(true);
            menu = _purchaseMenu;
            break;
        case VERBOSE: {
            auto tower = _world->_gameplayLayer->getTower(_currentTile);
            auto enabled = tower->getLevel() < 2 && _lastCoin >= tower->getCost();

            auto btn = static_cast<ui::Button *>(_verboseMenu->getChildByTag(UPGRADE));
            btn->setEnabled(enabled);
            btn->setBright(enabled);

            _verboseMenu->setVisible(true);
            menu = _verboseMenu;
            break;
        }
        case VALIDATION:
            _validationMenu->setVisible(true);
            menu = _validationMenu;
            break;
        default:
            break;
    }

    if (menu)
        for (auto btn : menu->getChildren()) {
            btn->setScale(0.f);
            btn->runAction(ScaleTo::create(0.15f, 1.f));
        }

    _state = state;
}

void WheelMenu::openAt(cocos2d::Vec2 position) {
    auto nodeValue = _world->_mapLayer->_grid.getNode(position);

    if (nodeValue == 0) {
        _currentTile = position;

        setPosition(algorithm::toCircularGrid(position));

        _world->_mapLayer->activateSlot(_currentTile);

        setState(PURCHASE);
    } else if (nodeValue == 1) {
        _currentTile = position;
        setPosition(algorithm::toCircularGrid(position));

        _world->_gameplayLayer->getTower(_currentTile)->setVerbose(true);

        setState(VERBOSE);
    }

    updateButtonStates();

    _world->_audioEngine->playEffect("audio/open.wav");
}

void WheelMenu::close() {
    auto nodeValue = _world->_mapLayer->_grid.getNode(_currentTile);

    if (nodeValue == 0) {
        _world->_mapLayer->deactivateSlot(_currentTile);
    } else if (nodeValue == 1) {
        _world->_gameplayLayer->getTower(_currentTile)->setVerbose(false);
    }

    _mock->setVisible(false);

    setState(IDLE);
}

void WheelMenu::updateButtonStates() {
    if (_state == PURCHASE) {
        ui::Button *btn;
        bool enabled;
        for (unsigned int i = 0; i < 3; ++i) {
            btn = static_cast<ui::Button *>(_purchaseMenu->getChildByTag(i));
            enabled = _lastCoin >= _models.at(i).at("base_cost").asFloat();
            btn->setEnabled(enabled);
            btn->setBright(enabled);
        }
    } else if (_state == VERBOSE) {
        auto tower = _world->_gameplayLayer->getTower(_currentTile);

        auto towerCost = tower->getCost();
        auto enabled = tower->getLevel() < 2 && _lastCoin >= towerCost;

        auto btn = static_cast<ui::Button *>(_verboseMenu->getChildByTag(UPGRADE));
        btn->setEnabled(enabled);
        btn->setBright(enabled);
    }
}

void WheelMenu::towerButtonCallback(Ref *sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        auto btn = static_cast<ui::Button *>(sender);
        _selectedType = static_cast<ModelID>(btn->getTag());

        _mock->setVisible(true);
        _mock->setScale(1.f);
        _mock->setSkin(_selectedType);

        setState(VALIDATION);

        btn = static_cast<ui::Button *>(_validationMenu->getChildByTag(ACCEPT));

        //Bind tower placement function as accept button callback
        btn->addTouchEventListener([&](Ref *__sender, ui::Widget::TouchEventType __type) {
            if (__type == ui::Widget::TouchEventType::ENDED) {
                if (_world->placeTower(_selectedType, _currentTile)) {
                    close();
                    _world->_audioEngine->playEffect("audio/click.wav");
                }
            }
        });

        _world->_audioEngine->playEffect("audio/click.wav");
    }
}

void WheelMenu::sellButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        setState(VALIDATION);

        auto btn = static_cast<ui::Button *>(_validationMenu->getChildByTag(ACCEPT));

        //Bind tower placement function as accept button callback
        btn->addTouchEventListener([&](Ref *__sender, ui::Widget::TouchEventType __type) {
            if (__type == ui::Widget::TouchEventType::ENDED) {
                _world->destroyTower(_currentTile);
                _world->_audioEngine->playEffect("audio/click.wav");
                close();
            }
        });

        _world->_audioEngine->playEffect("audio/click.wav");
    }
}

void WheelMenu::upgradeButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        setState(VALIDATION);

        auto btn = static_cast<ui::Button *>(_validationMenu->getChildByTag(ACCEPT));

        //Bind tower upgrade function as accept button callback
        btn->addTouchEventListener([&](Ref *__sender, ui::Widget::TouchEventType __type) {
            if (__type == ui::Widget::TouchEventType::ENDED) {
                _world->upgradeTower(_currentTile);
                _world->_audioEngine->playEffect("audio/click.wav");
                close();
            }
        });

        _world->_audioEngine->playEffect("audio/click.wav");
    }
}

void WheelMenu::declineButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        _world->_audioEngine->playEffect("audio/click.wav");
        close();
    }
}
