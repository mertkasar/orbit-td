#include "CostIndicator.h"

#include <Globals.h>

#include <2d/CCSprite.h>
#include <2d/CCLabel.h>

#include <sstream>

USING_NS_CC;

CostIndicator::CostIndicator() {
    CCLOG("CostIndicator created");

}

CostIndicator::~CostIndicator() {
    CCLOG("CostIndicator deleted");
}

CostIndicator *CostIndicator::create() {
    CostIndicator *ret = new(std::nothrow) CostIndicator();

    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool CostIndicator::init() {
    if (!ui::Layout::init())
        return false;

    setCascadeOpacityEnabled(true);
    setCascadeColorEnabled(true);

    auto energySprite = Sprite::create("textures/energy.png");
    energySprite->setPosition(Vec2(-40.f, 0.f));
    addChild(energySprite);

    _costLabel = Label::createWithTTF("0", "fonts/kenvector_future.ttf", 36);
    _costLabel->setPosition(Vec2(10.f, 5.f));
    _costLabel->enableGlow(Color4B::YELLOW);
    addChild(_costLabel);

    return true;
}

void CostIndicator::update(int cost) {
    if (cost < 0) {
        setColor(Color::RED);
    } else if (cost > 0) {
        setColor(Color::GREEN);
    }

    std::stringstream ss;
    ss << std::abs(cost);

    _costLabel->setString(ss.str());
}
