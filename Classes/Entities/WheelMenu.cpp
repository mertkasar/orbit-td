#include "WheelMenu.h"

#include "Turret.h"
#include "TowerMock.h"
#include "CostIndicator.h"
#include "../Scenes/World.h"
#include "../Scenes/MapLayer.h"
#include "../Scenes/GameplayLayer.h"

#include <2d/CCMenu.h>
#include <2d/CCActionInterval.h>
#include <2d/CCSprite.h>
#include <ui/UIImageView.h>
#include <ui/UIButton.h>
#include <SimpleAudioEngine.h>

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

    auto item = ui::Button::create("textures/btn_turret_n.png", "textures/btn_turret_t.png", "");
    item->setTag(MACHINE_GUN);
    item->setPosition(Vec2(-SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    _purchaseMenu->addChild(item);

    item = ui::Button::create("textures/btn_laser_n.png", "textures/btn_laser_t.png", "");
    item->setTag(LASER_GUN);
    item->setPosition(Vec2(0, SHIFT));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    _purchaseMenu->addChild(item);

    item = ui::Button::create("textures/btn_rl_n.png", "textures/btn_rl_t.png", "");
    item->setTag(MISSILE_LAUNCHER);
    item->setPosition(Vec2(SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    _purchaseMenu->addChild(item);

    _verboseMenu = ui::Layout::create();

    item = ui::Button::create("textures/btn_upgrade_n.png", "textures/btn_upgrade_t.png", "");
    item->setTag(UPGRADE);
    item->setPosition(Vec2(-SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::upgradeButtonCallback, this));
    _verboseMenu->addChild(item);

    item = ui::Button::create("textures/btn_sell_n.png", "textures/btn_sell_t.png", "");
    item->setTag(SELL);
    item->setPosition(Vec2(SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::sellButtonCallback, this));
    _verboseMenu->addChild(item);

    _validationMenu = ui::Layout::create();

    item = ui::Button::create("textures/btn_accept_n.png", "textures/btn_accept_t.png", "");
    item->setTag(ACCEPT);
    item->setPosition(Vec2(SHIFT, 0.f));
    _validationMenu->addChild(item);

    item = ui::Button::create("textures/btn_decline_n.png", "textures/btn_decline_t.png", "");
    item->setTag(DECLINE);
    item->setPosition(Vec2(-SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::declineButtonCallback, this));
    _validationMenu->addChild(item);

    _mock = TowerMock::create();

    _energyCost = CostIndicator::create();
    _energyCost->update(-130);
    _energyCost->setPosition(Vec2(0.f, -60.f));
    _energyCost->setVisible(false);
    addChild(_energyCost);

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
    auto totalCoin = _world->_gameplayLayer->getTotalEnergy();

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
    _energyCost->setVisible(false);

    setState(IDLE);
}

void WheelMenu::updateButtonStates() {
    if (_state == PURCHASE) {
        ui::Button *btn;
        bool enabled;
        for (unsigned int i = 0; i < 3; ++i) {
            btn = static_cast<ui::Button *>(_purchaseMenu->getChildByTag(i));
            enabled = _lastCoin >= _world->getModel(i).at("base_cost").asFloat();
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
        auto &selectedModel = _world->getModel(_selectedType);

        _mock->setVisible(true);
        _mock->setScale(1.f);
        _mock->update(selectedModel);

        _energyCost->update(-selectedModel.at("base_cost").asInt());
        _energyCost->setVisible(true);

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

        auto cost = _world->_gameplayLayer->getTower(_currentTile)->getCost();
        _energyCost->update(cost);
        _energyCost->setVisible(true);

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

        auto cost = _world->_gameplayLayer->getTower(_currentTile)->getCost();
        _energyCost->update(-cost);
        _energyCost->setVisible(true);

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
