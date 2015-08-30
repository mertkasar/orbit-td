#include <UI/WheelMenu.h>

#include <2d/CCLayer.h>
#include <2d/CCMenu.h>
#include <ui/UIImageView.h>
#include <ui/UILayout.h>
#include <Utilities/Algorithm.h>

USING_NS_CC;

void WheelMenu::init(Layer *pLayer) {
    mRoot = ui::Layout::create();
    mRoot->setVisible(false);
    mRoot->setBackGroundImage("textures/ui/wheel_menu_bg.png");
    mRoot->setContentSize(mRoot->getBackGroundImageTextureSize());
    mRoot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    pLayer->addChild(mRoot);
}

void WheelMenu::openAt(cocos2d::Vec2 pPosition) {
    mRoot->setPosition(algorithm::toCircularGrid(pPosition));
    mRoot->setVisible(true);
}

void WheelMenu::close() {
    mRoot->setVisible(false);
}
