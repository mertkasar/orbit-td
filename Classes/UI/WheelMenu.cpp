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

    auto item = ui::Button::create("textures/ui/btn_trt.png", "");
    item->setTag(TURRET);
    item->setPosition(Vec2(-SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    mPurchaseMenu->addChild(item);

    item = ui::Button::create("textures/ui/btn_lsr.png", "");
    item->setTag(LASER);
    item->setPosition(Vec2(0, SHIFT));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    mPurchaseMenu->addChild(item);

    item = ui::Button::create("textures/ui/btn_rl.png", "");
    item->setTag(R_LAUNCHER);
    item->setPosition(Vec2(SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    mPurchaseMenu->addChild(item);

    mVerboseMenu = ui::Layout::create();

    item = ui::Button::create("textures/ui/btn_upgrade.png", "");
    item->setTag(UPGRADE);
    item->setPosition(Vec2(-SHIFT, 0.f));
    mVerboseMenu->addChild(item);

    item = ui::Button::create("textures/ui/btn_sell.png", "");
    item->setTag(SELL);
    item->setPosition(Vec2(SHIFT, 0.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::sellButtonCallback, this));
    mVerboseMenu->addChild(item);

    mValidationMenu = ui::Layout::create();

    item = ui::Button::create("textures/ui/btn_accept.png", "");
    item->setTag(ACCEPT);
    item->setPosition(Vec2(SHIFT, 0.f));
    mValidationMenu->addChild(item);

    item = ui::Button::create("textures/ui/btn_decline.png", "");
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
    if (mState == PURCHASE) {
        auto totalCoin = mWorld->getTotalCoin();

        if (totalCoin != mLastCoin) {
            auto btn = static_cast<ui::Button *>(mPurchaseMenu->getChildByTag(TURRET));
            bool enabled = totalCoin >= TURRET_COST;
            btn->setEnabled(enabled);
            btn->setBright(enabled);

            btn = static_cast<ui::Button *>(mPurchaseMenu->getChildByTag(LASER));
            enabled = totalCoin >= LASER_COST;
            btn->setEnabled(enabled);
            btn->setBright(enabled);

            btn = static_cast<ui::Button *>(mPurchaseMenu->getChildByTag(R_LAUNCHER));
            enabled = totalCoin >= R_LAUNCHER_COST;
            btn->setEnabled(enabled);
            btn->setBright(enabled);

            mLastCoin = totalCoin;
        }
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
        case VERBOSE:
            mVerboseMenu->setVisible(true);
            menu = mVerboseMenu;
            break;
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
