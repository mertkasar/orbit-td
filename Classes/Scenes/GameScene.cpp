#include <Scenes/GameScene.h>

#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <2d/CCActionInterval.h>
#include <2d/CCActionInstant.h>
#include <2d/CCDrawNode.h>
#include <physics/CCPhysicsWorld.h>
#include <physics/CCPhysicsContact.h>

#include <Globals.h>
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
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0, 0));

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
    connectListeners();

    return true;
}

void GameScene::buildScene() {
    mBackgroundLayer = LayerColor::create(Color4B(42, 45, 51, 255));
    mGameplayLayer = Layer::create();

    auto tower1 = Tower::create();
    tower1->setPosition(mCanvasCenter + Vec2(200.f, 100.f));

    auto tower2 = Tower::create();
    tower2->setPosition(mCanvasCenter - Vec2(200.f, 100.f));

    // Prepare path
    const float PI = 3.14159265f;
    float angle = 36.f;
    float radius = 200;
    Vec2 origin = tower1->getPosition();
    for (float alfa = 0; alfa < 360; alfa = alfa + angle) {
        Vec2 position = Vec2(radius * cos(alfa * PI / 180),
                             radius * sin(alfa * PI / 180));
        mPath.addNode(origin + position);
    }

    // Prepare visual representation of the path; Debug purposes only!
    auto canvas = DrawNode::create();
    auto waypoints = mPath.getWayPointList();
    for (unsigned int i = 0; i < waypoints.size(); i++) {
        canvas->drawSolidCircle(waypoints.at(i), 6.f, 0.f, 50, Color4F::RED);

        if (i != 0)
            canvas->drawLine(waypoints.at(i), waypoints.at(i - 1), Color4F::RED);
    }

    mGameplayLayer->addChild(canvas);
    mGameplayLayer->addChild(tower1);
    mGameplayLayer->addChild(tower2);

    this->addChild(mBackgroundLayer);
    this->addChild(mGameplayLayer);

    spawnEnemy(0.f);

    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::spawnEnemy), 1.f);

}

void GameScene::connectListeners() {
    // Add contact event listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [](PhysicsContact &pContact) {
        auto a = pContact.getShapeA()->getBody();
        auto b = pContact.getShapeB()->getBody();

        if ((a->getCategoryBitmask() == TOWER_RANGE_MASK && b->getCategoryBitmask() == ENEMY_MASK) ||
            (a->getCategoryBitmask() == ENEMY_MASK && b->getCategoryBitmask() == TOWER_RANGE_MASK)) {

            Tower *tower = nullptr;
            Enemy *enemy = nullptr;

            if (a->getCategoryBitmask() == TOWER_RANGE_MASK) {
                tower = static_cast<Tower *>(a->getNode());
                enemy = static_cast<Enemy *>(b->getNode());
            } else {
                tower = static_cast<Tower *>(b->getNode());
                enemy = static_cast<Enemy *>(a->getNode());
            }

            tower->addTarget(enemy);
        }

        return true;
    };
    contactListener->onContactSeparate = [](PhysicsContact &pContact) {
        auto a = pContact.getShapeA()->getBody();
        auto b = pContact.getShapeB()->getBody();

        if ((a->getCategoryBitmask() == TOWER_RANGE_MASK && b->getCategoryBitmask() == ENEMY_MASK) ||
            (a->getCategoryBitmask() == ENEMY_MASK && b->getCategoryBitmask() == TOWER_RANGE_MASK)) {

            Tower *tower = nullptr;
            Enemy *enemy = nullptr;

            if (a->getCategoryBitmask() == TOWER_RANGE_MASK) {
                tower = static_cast<Tower *>(a->getNode());
                enemy = static_cast<Enemy *>(b->getNode());
            } else {
                tower = static_cast<Tower *>(b->getNode());
                enemy = static_cast<Enemy *>(a->getNode());
            }

            tower->removeTarget(enemy);
        }
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void GameScene::spawnEnemy(float pDelta) {
    auto enemy = Enemy::create();
    enemy->setPosition(Vec2(mVisibleSize.width - 100.f, mVisibleSize.height / 2.f));
    enemy->constructPath(mPath);

    mGameplayLayer->addChild(enemy);
}
