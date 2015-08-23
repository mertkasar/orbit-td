#include <Scenes/GameScene.h>

#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <base/CCEventListenerTouch.h>
#include <2d/CCActionInterval.h>
#include <2d/CCActionInstant.h>
#include <2d/CCDrawNode.h>
#include <2d/CCLabel.h>
#include <physics/CCPhysicsWorld.h>
#include <physics/CCPhysicsContact.h>

#include <Entities/Enemy.h>
#include <Entities/Towers/Turret.h>
#include <Entities/Towers/Laser.h>
#include <Entities/Towers/RLauncher.h>

#include <sstream>

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

void GameScene::update(float pDelta) {
    //Clear dead enemy objects
    for (auto enemy : mEnemies)
        if (enemy->isDead()) {
            enemy->removeFromParent();
            mEnemies.eraseObject(enemy);
        }

    //Update log labels
    std::stringstream ss;
    ss << "Enemy pool has " << mEnemyPool.getSize() << " instances" << "\n"
    << "Missile pool has " << RLauncher::getMissilePoolSize() << " instance" << "\n"
    << "Enemy list has " << mEnemies.size() << " references";

    mLogLabel->setString(ss.str());
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
    if (mPath.isReached(traversed, START)) {
        mPath.construct(traversed, START, GOAL);
        drawPath();
    }

    mLogLabel = Label::createWithTTF("", "fonts/ubuntu.ttf", 32);
    mLogLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    mLogLabel->setPosition(mCanvasCenter - Vec2(220.f, 0.f));

    mGameplayLayer->addChild(mPathCanvas);
    mGameplayLayer->addChild(mLogLabel);

    spawnEnemy(0.f);
    placeTower(Vec2(1, 4));
    placeTower(Vec2(1, 5));
    placeTower(Vec2(1, 6));
    placeTower(Vec2(3, 4));
    placeTower(Vec2(3, 5));
    placeTower(Vec2(3, 6));

    this->addChild(mBackgroundLayer);
    this->addChild(mGameplayLayer);

    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::spawnEnemy), 2.f);
    this->scheduleUpdate();
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

        Vec2 touched = Vec2(-1, -1);
        Vec2 size = mGrid.getSize();
        for (int i = 0; i < size.x; i++) {
            for (int j = 0; j < size.y; j++) {
                Vec2 current = Vec2(i, j);
                if (mGrid.getNode(current) == 0) {
                    Vec2 location = algorithm::toCircularGrid(current);
                    Rect boundingBox = Rect(location.x - NODE_TOUCH_SIZE / 2.f,
                                            location.y - NODE_TOUCH_SIZE / 2.f,
                                            NODE_TOUCH_SIZE, NODE_TOUCH_SIZE);

                    if (boundingBox.containsPoint(touchLocation)) {
                        touched = current;
                        break;
                    }
                }
            }
        }

        if (touched.x > -1 && touched.y > -1) {
            Grid testGrid = mGrid;
            testGrid.setNode(touched, 1);

            auto traversed = algorithm::traverse(testGrid, START, GOAL);

            if (isAvailable(traversed, touched)) {
                placeTower(touched);
                mPath.construct(traversed, START, GOAL);
                drawPath();

                for (auto enemy : mEnemies) {
                    auto &enemyPath = enemy->getPath();
                    auto from = enemyPath.getCurrentWaypoint().tile;

                    enemyPath.construct(traversed, from, GOAL);
                }
            }
        } else
            CCLOG("You can't place a tower here!");

        return true;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void GameScene::spawnEnemy(float pDelta) {
    auto enemy = mEnemyPool.fetch();
    Vec2 spawnPosition = mGameplayLayer->convertToNodeSpace(Vec2(mVisibleSize.width - 50.f, mVisibleSize.height / 2.f));
    enemy->ignite(spawnPosition, mPath);

    mGameplayLayer->addChild(enemy);
    mEnemies.pushBack(enemy);
}

bool GameScene::isAvailable(const TraverseData &pTraversed, cocos2d::Vec2 pTile) {
    if (!mPath.isReached(pTraversed, START)) {
        CCLOG("You can't block the path!");
        return false;
    }

    for (auto enemy : mEnemies) {
        auto current = enemy->getPath().getCurrentWaypoint().tile;

        if ((current == pTile) || !enemy->getPath().isReached(pTraversed, current)) {
            CCLOG("You can't block enemies!");
            return false;
        }
    }

    return true;
}

void GameScene::placeTower(Vec2 pTile) {
    auto tower = RLauncher::create();

    auto position = algorithm::toCircularGrid(pTile);
    tower->setPosition(position);

    mGameplayLayer->addChild(tower);
    mGrid.setNode(pTile, 1);
}

void GameScene::drawPath() {
    mPathCanvas->clear();
    auto waypoints = mPath.getWayPoints();

    for (unsigned int i = 0; i < waypoints.size(); i++) {
        auto waypoint = waypoints.at(i);

        mPathCanvas->drawSolidCircle(waypoint.location, 6.f, 0.f, 50, Color4F::RED);
        mPathCanvas->drawCircle(waypoint.location, waypoint.reachRadius, 0.f, 50, false, Color4F::RED);

        if (i != 0) {
            auto previousWaypoint = waypoints.at(i - 1);
            mPathCanvas->drawLine(waypoint.location, previousWaypoint.location, Color4F::RED);
        }
    }
}
