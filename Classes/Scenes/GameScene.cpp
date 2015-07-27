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
#include <Utilities/Algorithm.h>

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
    mGameplayLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    mGameplayLayer->setPosition(Vec2(-300.f, mVisibleSize.height / 2.f));
    mGameplayLayer->setScale(0.85f);

    // Prepare sample grid
    mGrid.create(Vec2(7, 10));
    for (int i = 0; i < mGrid.getSize().x; i++)
        mGrid.setNode(Vec2(i, 0), 1);
    mGrid.setNode(Vec2(3, 0), 0);

    // Draw grid
    auto grid = DrawNode::create();

    Vec2 size = mGrid.getSize();
    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            Color4F color;
            if (mGrid.getNode(Vec2(i, j)) == 0)
                color = Color4F::GREEN;
            else
                color = Color4F::RED;

            grid->drawSolidCircle(algorithm::toCircularGrid(Vec2(i, j)), 5.f, 0.f, 50, color);
        }
    }

    grid->drawSolidCircle(Vec2(-80.f, 0.f), 600.f, 0.f, 50, Color4F::BLUE);

    // Construct test path
    mPath.addNode(algorithm::toCircularGrid(Vec2(3, 9)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(2, 9)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(1, 9)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(0, 9)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(0, 8)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(0, 7)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(1, 7)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(2, 7)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(3, 7)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(4, 7)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(5, 7)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(6, 7)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(6, 6)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(6, 5)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(5, 5)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(4, 5)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(3, 5)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(2, 5)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(1, 5)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(0, 5)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(0, 4)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(0, 3)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(1, 3)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(2, 3)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(3, 3)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(3, 2)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(3, 1)));
    mPath.addNode(algorithm::toCircularGrid(Vec2(3, 0)));

    auto path = DrawNode::create();
    auto waypoints = mPath.getWayPointList();
    for (unsigned int i = 0; i < waypoints.size(); i++) {
        path->drawSolidCircle(waypoints.at(i), 6.f, 0.f, 50, Color4F::RED);

        if (i != 0)
            path->drawLine(waypoints.at(i), waypoints.at(i - 1), Color4F::RED);
    }

    mGameplayLayer->addChild(grid);
    mGameplayLayer->addChild(path);

    placeTower(Vec2(1, 8));
    placeTower(Vec2(0, 6));
    placeTower(Vec2(1, 6));
    placeTower(Vec2(2, 2));

    this->addChild(mBackgroundLayer);
    this->addChild(mGameplayLayer);

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
    enemy->setPosition(mPath.getCurrentNode());
    enemy->constructPath(mPath);

    mGameplayLayer->addChild(enemy);
}

void GameScene::placeTower(Vec2 pTile) {
    auto tower = Tower::create();

    auto position = algorithm::toCircularGrid(pTile);
    tower->setPosition(position);

    mGameplayLayer->addChild(tower);
    mGrid.setNode(pTile, 1);
}