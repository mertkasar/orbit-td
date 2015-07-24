#include <Scenes/GameScene.h>

#include <base/CCDirector.h>
#include <2d/CCActionInterval.h>

#include <Entities/Tower.h>
#include <Entities/Enemy.h>

USING_NS_CC;

GameScene::GameScene() {
    CCLOG("GameScene created");
}

GameScene::~GameScene() {
    CCLOG("GameScene deleted");
}

Scene *GameScene::createScene() {
    auto scene = Scene::create();

    auto layer = GameScene::create();

    scene->addChild(layer);

    return scene;
}

bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }

    mVisibleSize = Director::getInstance()->getVisibleSize();
    mOrigin = Director::getInstance()->getVisibleOrigin();
    mCanvasCenter = Vec2(mVisibleSize / 2.f) + mOrigin;

    buildScene();

    return true;
}

void GameScene::buildScene() {
    mBackgroundLayer = LayerColor::create(Color4B(42, 45, 51, 255));
    mGameplayLayer = Layer::create();

    auto tower1 = Tower::create();
    tower1->setPosition(mCanvasCenter + Vec2(200.f, 100.f));

    auto tower2 = Tower::create();
    tower2->setPosition(mCanvasCenter - Vec2(200.f, 100.f));

    auto enemy = Enemy::create();
    enemy->setPosition(Vec2(mVisibleSize.width - 100.f, mVisibleSize.height / 2.f));
    enemy->setRotation(-90.f);
    enemy->runAction(MoveTo::create(10.f, Vec2(100, mVisibleSize.height / 2.f)));

    mGameplayLayer->addChild(tower1);
    mGameplayLayer->addChild(tower2);
    mGameplayLayer->addChild(enemy);

    this->addChild(mBackgroundLayer);
    this->addChild(mGameplayLayer);
}
