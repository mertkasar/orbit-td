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

#include <Entities/Creeps/Creep.h>
#include <Entities/Towers/Turret.h>
#include <Entities/Towers/Laser.h>
#include <Entities/Towers/RLauncher.h>

#include <sstream>

USING_NS_CC;

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

    mTotalCoin = STARTING_COIN;
    mLife = STARTING_LIFE;

    buildScene();
    connectListeners();

    mWaveDirector.init(this);

    return true;
}

void GameScene::update(float pDelta) {
    //Clear dead enemy objects
    for (auto enemy : mCreeps)
        if (enemy->isDead()) {
            enemy->removeFromParent();
            mCreeps.eraseObject(enemy);

            if (enemy->isKilled())
                mTotalCoin = mTotalCoin + enemy->getReward();
            else if (enemy->isReachedEnd())
                mLife = mLife - 1;
        }

    mHUD.update(pDelta);
    mWheelMenu.update(pDelta);

    mWaveDirector.update(pDelta);

    if (mLife <= 0) {
        mHUD.notify('I', "Game Over!");
        this->unscheduleUpdate();
    }

    if (mWaveDirector.isCleared()) {
        mHUD.notify('I', "All waves are cleared!");
        this->unscheduleUpdate();
    }
}

void GameScene::spawnEnemy(unsigned int pType, int pOrder) {
    auto enemy = mCreepPool.fetch();
    Vec2 spawnPosition = mGameplayLayer->convertToNodeSpace(Vec2(mVisibleSize.width - 50.f, mVisibleSize.height / 2.f));
    spawnPosition = spawnPosition + Vec2(pOrder * 100, 0);
    enemy->ignite(spawnPosition, mPath);

    mGameplayLayer->addChild(enemy);
    mCreeps.pushBack(enemy);
}

bool GameScene::placeTower(unsigned int pType, Vec2 pTile) {
    Grid testGrid = mGrid;
    testGrid.setNode(pTile, 1);

    auto traversed = algorithm::traverse(testGrid, mStart, mGoal);

    if (isAvailable(traversed, pTile)) {
        auto position = algorithm::toCircularGrid(pTile);
        Tower *newTower;

        switch (pType) {
            case 1:
                newTower = Turret::create();
                break;
            case 2:
                newTower = Laser::create();
                break;
            case 3:
                newTower = RLauncher::create();
                break;
            default:
                break;
        }

        if (newTower) {
            newTower->setPosition(position);

            mGameplayLayer->addChild(newTower);
            mGrid.setNode(pTile, 1);

            mPath.construct(traversed, mStart, mGoal);
            drawPath();

            for (auto enemy : mCreeps) {
                auto &enemyPath = enemy->getPath();
                auto from = enemyPath.getCurrentWaypoint().tile;

                enemyPath.construct(traversed, from, mGoal);
            }

            mTotalCoin = mTotalCoin - newTower->getCost();

            return true;
        }
    }

    return false;
}

void GameScene::buildScene() {
    mBackgroundLayer = LayerColor::create(Color4B(42, 45, 51, 255));

    mGameplayLayer = Layer::create();
    mGameplayLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    // Prepare sample grid
    mGrid.create(Vec2(5, 10));
    for (int i = 0; i < mGrid.getSize().x; i++)
        mGrid.setNode(Vec2(i, 0), 1);
    mGrid.setNode(Vec2(2, 0), 0);

    mStart = Vec2(2, 9);
    mGoal = Vec2(2, 0);

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

    grid->drawSolidCircle(Vec2(-480.f, 360.f), 600.f, 0.f, 50, Color4F::BLUE);

    mGameplayLayer->addChild(grid);

    mPathCanvas = DrawNode::create();

    auto traversed = algorithm::traverse(mGrid, mStart, mGoal);
    if (mPath.isReached(traversed, mStart)) {
        mPath.construct(traversed, mStart, mGoal);
        drawPath();
    }

    mGameplayLayer->addChild(mPathCanvas);

    mUILayer = Layer::create();
    mHUD.init(mUILayer, this);
    mWheelMenu.init(mUILayer, this);

    mHUD.notify('I', "Game is starting!", 2.f);

    this->addChild(mBackgroundLayer);
    this->addChild(mGameplayLayer);
    this->addChild(mUILayer);

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
            Creep *enemy = nullptr;

            if (a->getCategoryBitmask() == TOWER_RANGE_MASK) {
                tower = static_cast<Tower *>(a->getNode());
                enemy = static_cast<Creep *>(b->getNode());
            } else {
                tower = static_cast<Tower *>(b->getNode());
                enemy = static_cast<Creep *>(a->getNode());
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
            Creep *enemy = nullptr;

            if (a->getCategoryBitmask() == TOWER_RANGE_MASK) {
                tower = static_cast<Tower *>(a->getNode());
                enemy = static_cast<Creep *>(b->getNode());
            } else {
                tower = static_cast<Tower *>(b->getNode());
                enemy = static_cast<Creep *>(a->getNode());
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

        if (touched.x > -1 && touched.y > -1 && !mWheelMenu.isOpen()) {
            mWheelMenu.openAt(touched);
        } else {
            mWheelMenu.close();
        }

        return true;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool GameScene::isAvailable(const TraverseData &pTraversed, cocos2d::Vec2 pTile) {
    if (!mPath.isReached(pTraversed, mStart)) {
        mHUD.notify('E', "You can't block the path!");
        return false;
    }

    for (auto enemy : mCreeps) {
        auto current = enemy->getPath().getCurrentWaypoint().tile;

        if ((current == pTile) || !enemy->getPath().isReached(pTraversed, current)) {
            mHUD.notify('E', "You can't block enemies!");
            return false;
        }
    }

    return true;
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
