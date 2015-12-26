#include <UI/WheelMenu.h>

#include <2d/CCMenu.h>
#include <2d/CCActionInterval.h>
#include <ui/UIImageView.h>
#include <ui/UIButton.h>

#include <Scenes/World.h>
#include <Scenes/MapLayer.h>
#include <Scenes/GameplayLayer.h>
#include <Entities/Towers/Tower.h>

USING_NS_CC;

void WheelMenu::init(Layer *pLayer, World *pGameScene) {
    mWorld = pGameScene;

    mCurrentTile = Vec2(-1, -1);
    mLastCoin = 0;

    mRoot = Node::create();
    mRoot->setContentSize(Size(250.f, 250.f));

    mPurchaseMenu = ui::Layout::create();

    const float SHIFT = 90.f;

    auto item = ui::Button::create("btn_trt.png", "", "", ui::Widget::TextureResType::PLIST);
    item->setTag(TURRET);
    item->setPosition(Vec2(-SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    mPurchaseMenu->addChild(item);

    item = ui::Button::create("btn_lsr.png", "", "", ui::Widget::TextureResType::PLIST);
    item->setTag(LASER);
    item->setPosition(Vec2(0, SHIFT));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    mPurchaseMenu->addChild(item);

    item = ui::Button::create("btn_rl.png", "", "", ui::Widget::TextureResType::PLIST);
    item->setTag(R_LAUNCHER);
    item->setPosition(Vec2(SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    mPurchaseMenu->addChild(item);

    mVerboseMenu = ui::Layout::create();

    item = ui::Button::create("btn_upgrade.png", "", "", ui::Widget::TextureResType::PLIST);
    item->setTag(UPGRADE);
    item->setPosition(Vec2(-SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::upgradeButtonCallback, this));
    mVerboseMenu->addChild(item);

    item = ui::Button::create("btn_sell.png", "", "", ui::Widget::TextureResType::PLIST);
    item->setTag(SELL);
    item->setPosition(Vec2(SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::sellButtonCallback, this));
    mVerboseMenu->addChild(item);

    mValidationMenu = ui::Layout::create();

    item = ui::Button::create("btn_accept.png", "", "", ui::Widget::TextureResType::PLIST);
    item->setTag(ACCEPT);
    item->setPosition(Vec2(SHIFT, 0.f));
    mValidationMenu->addChild(item);

    item = ui::Button::create("btn_decline.png", "", "", ui::Widget::TextureResType::PLIST);
    item->setTag(DECLINE);
    item->setPosition(Vec2(-SHIFT, 0.f));
    mValidationMenu->addChild(item);

    mRoot->addChild(mPurchaseMenu);
    mRoot->addChild(mVerboseMenu);
    mRoot->addChild(mValidationMenu);

    pLayer->addChild(mRoot);

    setState(IDLE);
}

void WheelMenu::update(float pDelta) {
    auto totalCoin = mWorld->getTotalCoin();

    if (totalCoin != mLastCoin) {
        mLastCoin = totalCoin;

        updateButtonStates();
    }
}

void WheelMenu::setState(WheelMenu::State pState) {
    for (auto menu :  mRoot->getChildren())
        menu->setVisible(false);

    Node *menu = nullptr;

    switch (pState) {
        case IDLE:
            break;
        case PURCHASE:
            mPurchaseMenu->setVisible(true);
            menu = mPurchaseMenu;
            break;
        case VERBOSE: {
            auto tower = mWorld->gameplayLayer->getTower(mCurrentTile);
            auto enabled = tower->getLevel() < 2 && mLastCoin >= tower->getCost();

            auto btn = static_cast<ui::Button *>(mVerboseMenu->getChildByTag(UPGRADE));
            btn->setEnabled(enabled);
            btn->setBright(enabled);

            mVerboseMenu->setVisible(true);
            menu = mVerboseMenu;
            break;
        }
        case VALIDATION:
            mValidationMenu->setVisible(true);
            menu = mValidationMenu;
            break;
        default:
            break;
    }

    if (menu)
        for (auto btn : menu->getChildren()) {
            btn->setScale(0.f);
            btn->runAction(ScaleTo::create(0.15f, 1.f));
        }

    mState = pState;
}

void WheelMenu::openAt(cocos2d::Vec2 pPosition) {
    auto nodeValue = mWorld->mGrid.getNode(pPosition);

    if (nodeValue == 0) {
        mCurrentTile = pPosition;
        mRoot->setPosition(algorithm::toCircularGrid(pPosition));

        mWorld->mapLayer->activateSlot(mCurrentTile);

        setState(PURCHASE);
    } else if (nodeValue == 1) {
        mCurrentTile = pPosition;
        mRoot->setPosition(algorithm::toCircularGrid(pPosition));

        mWorld->gameplayLayer->getTower(mCurrentTile)->setVerbose(true);

        setState(VERBOSE);
    }

    updateButtonStates();
}

void WheelMenu::close() {
    auto nodeValue = mWorld->mGrid.getNode(mCurrentTile);

    if (nodeValue == 0) {
        mWorld->mapLayer->deactivateSlot(mCurrentTile);
    } else if (nodeValue == 1) {
        mWorld->gameplayLayer->getTower(mCurrentTile)->setVerbose(false);
    }

    setState(IDLE);
}

void WheelMenu::updateButtonStates() {
    if (mState == PURCHASE) {
        ui::Button *btn;
        bool enabled;
        for (unsigned int i = 0; i < 3; ++i) {
            btn = static_cast<ui::Button *>(mPurchaseMenu->getChildByTag(i));
            enabled = mLastCoin >= models.at(i).baseCost;
            btn->setEnabled(enabled);
            btn->setBright(enabled);
        }
    } else if (mState == VERBOSE) {
        auto tower = mWorld->gameplayLayer->getTower(mCurrentTile);

        auto towerCost = tower->getCost();
        auto enabled = tower->getLevel() < 2 && mLastCoin >= towerCost;

        auto btn = static_cast<ui::Button *>(mVerboseMenu->getChildByTag(UPGRADE));
        btn->setEnabled(enabled);
        btn->setBright(enabled);
    }
}

void WheelMenu::towerButtonCallback(Ref *pSender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        auto btn = static_cast<ui::Button *>(pSender);
        mSelectedType = static_cast<TowerTypes>(btn->getTag());

        mWorld->gameplayLayer->createMock(mSelectedType, mCurrentTile);

        setState(VALIDATION);

        btn = static_cast<ui::Button *>(mValidationMenu->getChildByTag(ACCEPT));

        //Bind tower placement function as accept button callback
        btn->addTouchEventListener([&](Ref *p_Sender, ui::Widget::TouchEventType p_Type) {
            if (p_Type == ui::Widget::TouchEventType::ENDED) {
                if (mWorld->placeTower(mSelectedType, mCurrentTile)) {
                    close();
                }
            }
        });

        btn = static_cast<ui::Button *>(mValidationMenu->getChildByTag(DECLINE));

        btn->addTouchEventListener([&](Ref *p_Sender, ui::Widget::TouchEventType p_Type) {
            if (p_Type == ui::Widget::TouchEventType::ENDED) {
                close();
                mWorld->gameplayLayer->removeMock();
            }
        });
    }
}

void WheelMenu::sellButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        setState(VALIDATION);

        auto btn = static_cast<ui::Button *>(mValidationMenu->getChildByTag(ACCEPT));

        //Bind tower placement function as accept button callback
        btn->addTouchEventListener([&](Ref *p_Sender, ui::Widget::TouchEventType p_Type) {
            if (p_Type == ui::Widget::TouchEventType::ENDED) {
                mWorld->destroyTower(mCurrentTile);
                close();
            }
        });

        btn = static_cast<ui::Button *>(mValidationMenu->getChildByTag(DECLINE));

        btn->addTouchEventListener([&](Ref *p_Sender, ui::Widget::TouchEventType p_Type) {
            if (p_Type == ui::Widget::TouchEventType::ENDED) {
                close();
            }
        });
    }
}

void WheelMenu::upgradeButtonCallback(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        setState(VALIDATION);

        auto btn = static_cast<ui::Button *>(mValidationMenu->getChildByTag(ACCEPT));

        //Bind tower upgrade function as accept button callback
        btn->addTouchEventListener([&](Ref *p_Sender, ui::Widget::TouchEventType p_Type) {
            if (p_Type == ui::Widget::TouchEventType::ENDED) {
                mWorld->upgradeTower(mCurrentTile);
                close();
            }
        });

        btn = static_cast<ui::Button *>(mValidationMenu->getChildByTag(DECLINE));

        btn->addTouchEventListener([&](Ref *p_Sender, ui::Widget::TouchEventType p_Type) {
            if (p_Type == ui::Widget::TouchEventType::ENDED) {
                close();
            }
        });
    }
}
