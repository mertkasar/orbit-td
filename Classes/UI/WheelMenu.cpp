#include <UI/WheelMenu.h>

#include <2d/CCMenu.h>
#include <ui/UIImageView.h>
#include <ui/UIButton.h>

#include <Scenes/GameScene.h>

USING_NS_CC;

void WheelMenu::init(Layer *pLayer, GameScene *pGameScene) {
    mGameScene = pGameScene;

    mOpen = false;

    mRoot = Node::create();
    mRoot->setVisible(false);
    mRoot->setContentSize(Size(250.f, 250.f));
    mRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    mTowerMenu = ui::Layout::create();

    auto item = ui::Button::create("textures/ui/btn_tower_trt.png", "");
    item->setName("#turret_button");
    item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    item->setRotation(90.f);
    item->setPosition(Vec2(mRoot->getContentSize().width / 2.f, mRoot->getContentSize().height / 2.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    mTowerMenu->addChild(item);

    item = ui::Button::create("textures/ui/btn_tower_lsr.png", "");
    item->setName("#laser_button");
    item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    item->setRotation(180.f);
    item->setPosition(Vec2(mRoot->getContentSize().width / 2.f, mRoot->getContentSize().height / 2.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    mTowerMenu->addChild(item);

    item = ui::Button::create("textures/ui/btn_tower_rl.png", "");
    item->setName("#rocketL_button");
    item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    item->setRotation(270.f);
    item->setPosition(Vec2(mRoot->getContentSize().width / 2.f, mRoot->getContentSize().height / 2.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    mTowerMenu->addChild(item);

    mRoot->addChild(mTowerMenu);

    pLayer->addChild(mRoot);
}

void WheelMenu::update(float pDelta) {
    auto totalCoin = mGameScene->getTotalCoin();

    auto btn = static_cast<ui::Button *>(mTowerMenu->getChildByName("#turret_button"));
    bool enabled = totalCoin >= TURRET_COST;
    btn->setEnabled(enabled);
    btn->setBright(enabled);

    btn = static_cast<ui::Button *>(mTowerMenu->getChildByName("#laser_button"));
    enabled = totalCoin >= LASER_COST;
    btn->setEnabled(enabled);
    btn->setBright(enabled);

    btn = static_cast<ui::Button *>(mTowerMenu->getChildByName("#rocketL_button"));
    enabled = totalCoin >= R_LAUNCHER_COST;
    btn->setEnabled(enabled);
    btn->setBright(enabled);
}

void WheelMenu::openAt(cocos2d::Vec2 pPosition) {
    mCurrentTile = pPosition;
    mRoot->setPosition(algorithm::toCircularGrid(pPosition));
    mRoot->setVisible(true);
    mOpen = true;
}

void WheelMenu::close() {
    mRoot->setVisible(false);
    mOpen = false;
}

void WheelMenu::towerButtonCallback(cocos2d::Ref *pSender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        auto btn_name = static_cast<ui::Button *>(pSender)->getName();
        TowerTypes type;

        if (btn_name == "#turret_button") {
            type = TURRET;
        } else if (btn_name == "#laser_button") {
            type = LASER;
        } else if (btn_name == "#rocketL_button") {
            type = R_LAUNCHER;
        }

        if (mGameScene->placeTower(type, mCurrentTile))
            close();
    }
}
