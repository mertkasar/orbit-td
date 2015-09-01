#include <UI/WheelMenu.h>

#include <2d/CCMenu.h>
#include <ui/UIImageView.h>
#include <ui/UIButton.h>

#include <Scenes/GameScene.h>

USING_NS_CC;

void WheelMenu::init(Layer *pLayer, GameScene *pGameScene) {
    mGameScene = pGameScene;

    mRoot = Node::create();
    mRoot->setVisible(false);
    mRoot->setContentSize(Size(250.f, 250.f));
    mRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    mTowerMenu = ui::Layout::create();

    auto item = ui::Button::create("textures/ui/btn_tower_trt.png", "");
    item->setName("turret_button");
    item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    item->setRotation(90.f);
    item->setPosition(Vec2(mRoot->getContentSize().width / 2.f, mRoot->getContentSize().height / 2.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    mTowerMenu->addChild(item);

    item = ui::Button::create("textures/ui/btn_tower_lsr.png", "");
    item->setName("laser_button");
    item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    item->setRotation(180.f);
    item->setPosition(Vec2(mRoot->getContentSize().width / 2.f, mRoot->getContentSize().height / 2.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    mTowerMenu->addChild(item);

    item = ui::Button::create("textures/ui/btn_tower_rl.png", "");
    item->setName("rocketL_button");
    item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    item->setRotation(270.f);
    item->setPosition(Vec2(mRoot->getContentSize().width / 2.f, mRoot->getContentSize().height / 2.f));
    item->addTouchEventListener(CC_CALLBACK_2(WheelMenu::towerButtonCallback, this));
    mTowerMenu->addChild(item);

    mRoot->addChild(mTowerMenu);

    pLayer->addChild(mRoot);
}

void WheelMenu::openAt(cocos2d::Vec2 pPosition) {
    mCurrentTile = pPosition;
    mRoot->setPosition(algorithm::toCircularGrid(pPosition));
    mRoot->setVisible(true);
}

void WheelMenu::close() {
    mRoot->setVisible(false);
}

void WheelMenu::towerButtonCallback(cocos2d::Ref *pSender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        auto btn_name = static_cast<ui::Button *>(pSender)->getName();
        unsigned int towerType = 0;

        if (btn_name == "turret_button") {
            towerType = 1;
        } else if (btn_name == "laser_button") {
            towerType = 2;
        } else if (btn_name == "rocketL_button") {
            towerType = 3;
        }

        if (mGameScene->placeTower(towerType, mCurrentTile))
            close();
    }
}
