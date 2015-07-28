#include <Scenes/GameScene.h>

#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <base/CCEventListenerTouch.h>
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

const Vec2 START = Vec2(2, 9);
const Vec2 GOAL = Vec2(2, 0);

GameScene::GameScene() {
    CCLOG("GameScene created");
}

GameScene::~GameScene() {
    CCLOG("GameScene deleted");
}

Scene *GameScene::createScene() {
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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
    mGameplayLayer->setPosition(Vec2(-400.f, mVisibleSize.height / 2.f));

    // Prepare sample grid
    mGrid.create(Vec2(5, 10));
    for (int i = 0; i < mGrid.getSize().x; i++)
        mGrid.setNode(Vec2(i, 0), 1);
    mGrid.setNode(Vec2(2, 0), 0);

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

    mGameplayLayer->addChild(grid);

    mPathCanvas = DrawNode::create();
    auto traversed = algorithm::traverse(mGrid, START, GOAL);
    constructPath(traversed, START, GOAL);

    mGameplayLayer->addChild(mPathCanvas);

    this->addChild(mBackgroundLayer);
    this->addChild(mGameplayLayer);

    /*spawnEnemy(0.f);
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::spawnEnemy), 2.f);*/
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

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch *pTouch, Event *pEvent) {
        auto touchLocation = mGameplayLayer->convertTouchToNodeSpace(pTouch);

        Vec2 size = mGrid.getSize();
        for (int i = 0; i < size.x; i++) {
            for (int j = 0; j < size.y; j++) {
                Vec2 tile = Vec2(i, j);
                if (mGrid.getNode(tile) == 0) {
                    Vec2 location = algorithm::toCircularGrid(tile);
                    Rect boundingBox = Rect(location.x - NODE_TOUCH_SIZE / 2.f,
                                            location.y - NODE_TOUCH_SIZE / 2.f,
                                            NODE_TOUCH_SIZE, NODE_TOUCH_SIZE);

                    if (boundingBox.containsPoint(touchLocation)) {
                        Grid testGrid = mGrid;
                        testGrid.setNode(tile, 1);

                        auto traversed = algorithm::traverse(testGrid, START, GOAL);

                        if (algorithm::isReached(traversed, GOAL)) {
                            placeTower(Vec2(i, j));
                            constructPath(traversed, START, GOAL);

                            CCLOG("Tower placed!");
                        } else {
                            CCLOG("You can't block the path!");
                        }
                    }
                }
            }
        }

        return true;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void GameScene::spawnEnemy(float pDelta) {
    auto enemy = Enemy::create();
    Vec2 spawnPosition = Vec2(mVisibleSize.width - 50.f, mVisibleSize.height / 2.f);
    enemy->setPosition(mGameplayLayer->convertToNodeSpace(spawnPosition));
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

void GameScene::constructPath(const TraverseData &pTraversed, const cocos2d::Vec2 pStart,
                              const cocos2d::Vec2 pGoal) {
    auto waypoints = algorithm::calculatePath(pTraversed, pStart, pGoal);

    mPath.clear();
    mPathCanvas->clear();
    for (unsigned int i = 0; i < waypoints.size(); i++) {
        auto waypoint = algorithm::toCircularGrid(waypoints.at(i));
        auto reachDensity = DEFAULT_WAYPOINT_DENSITY;

        mPath.addWaypoint(waypoint, reachDensity);

        mPathCanvas->drawSolidCircle(waypoint, 6.f, 0.f, 50, Color4F::RED);
        mPathCanvas->drawCircle(waypoint, reachDensity, 0.f, 50, false, Color4F::RED);

        if (i != 0) {
            auto previousWaypoint = algorithm::toCircularGrid(waypoints.at(i - 1));
            mPathCanvas->drawLine(waypoint, previousWaypoint, Color4F::RED);
        }
    }
}