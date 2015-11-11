#include "World.h"

#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <base/CCEventListenerTouch.h>
#include <2d/CCActionInterval.h>
#include <2d/CCActionInstant.h>
#include <2d/CCDrawNode.h>
#include <2d/CCLabel.h>
#include <physics/CCPhysicsWorld.h>
#include <physics/CCPhysicsContact.h>
#include <ui/UIImageView.h>

#include <Scenes/MapLayer.h>
#include <Scenes/GameplayLayer.h>
#include <Scenes/HUDLayer.h>
#include <Entities/Towers/Tower.h>
#include <Entities/Creep.h>

#include <sstream>

USING_NS_CC;

World::World() {
    CCLOG("World created");
}

World::~World() {
    CCLOG("World deleted");
}

Scene *World::createScene() {
    auto scene = Scene::createWithPhysics();

    auto physicsWorld = scene->getPhysicsWorld();
    physicsWorld->setGravity(Vect(0, 0));
    //physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = World::create();
    layer->setPhysicsWorld(physicsWorld);

    scene->addChild(layer);

    return scene;
}

bool World::init() {
    if (!Layer::init()) {
        return false;
    }

    mVisibleSize = Director::getInstance()->getVisibleSize();
    mOrigin = Director::getInstance()->getVisibleOrigin();
    mCanvasCenter = Vec2(mVisibleSize / 2.f) + mOrigin;

    mTotalCoin = STARTING_COIN;
    mLife = STARTING_LIFE;

    colors.push_back(Color::GREEN);
    colors.push_back(Color::YELLOW);
    colors.push_back(Color::BLUE);

    buildScene();
    connectListeners();

    //Init waves
    mWaves.clear();
    mWaves.push_back(std::vector<CreepTypes>{SPEEDY, RAPTOR, PULSAR, PANZER});
    mWaves.push_back(std::vector<CreepTypes>{RAPTOR});
    mWaves.push_back(std::vector<CreepTypes>{RAPTOR, RAPTOR, RAPTOR});
    mWaves.push_back(std::vector<CreepTypes>{SPEEDY, SPEEDY, RAPTOR, RAPTOR, RAPTOR});
    mWaves.push_back(std::vector<CreepTypes>{RAPTOR, RAPTOR, PULSAR, PULSAR});
    mWaves.push_back(std::vector<CreepTypes>{RAPTOR, RAPTOR, RAPTOR, SPEEDY, SPEEDY, PULSAR, PANZER});
    mWaves.push_back(std::vector<CreepTypes>(7, SPEEDY));
    mWaves.push_back(std::vector<CreepTypes>{PULSAR, PULSAR, PULSAR, PULSAR, PULSAR, PULSAR, PANZER});
    mWaves.push_back(std::vector<CreepTypes>{10, PULSAR});
    mWaves.push_back(std::vector<CreepTypes>{SPEEDY, SPEEDY, SPEEDY, SPEEDY, RAPTOR, RAPTOR, RAPTOR, RAPTOR, PULSAR,
                                             PULSAR, PANZER, PANZER});
    mWaves.push_back(std::vector<CreepTypes>{15, PANZER});

    mCurrentWave = 0;
    mCleared = false;

    return true;
}

void World::update(float pDelta) {
    mWheelMenu.update(pDelta);

    if (gameplayLayer->getCreepList().size() <= 0) {
        if (!spawnNextWave())
            mCleared = true;
    }

    if (mLife <= 0) {
        hudLayer->notify('I', "Game Over!");
        this->unscheduleUpdate();
    }

    if (isCleared()) {
        hudLayer->notify('I', "All waves are cleared!");
        this->unscheduleUpdate();
    }
}

bool World::placeTower(TowerTypes pType, Vec2 pTile) {
    Grid testGrid = mGrid;
    testGrid.setNode(pTile, 1);

    auto traversed = algorithm::traverse(testGrid, mStart, mGoal);

    if (isAvailable(traversed, pTile)) {
        gameplayLayer->buildMock(pTile);

        mapLayer->activateSlot(pTile);

        mPath.construct(traversed, mStart, mGoal);
        drawPath();

        for (auto enemy : gameplayLayer->getCreepList()) {
            auto &enemyPath = enemy->getPath();
            auto from = enemyPath.getCurrentWaypoint().tile;

            enemyPath.construct(traversed, from, mGoal);
        }

        mGrid.setNode(pTile, 1);

        return true;
    }

    return false;
}

void World::destroyTower(Vec2 pTile) {
    gameplayLayer->deleteTower(pTile);
    mGrid.setNode(pTile, 0);

    auto traversed = algorithm::traverse(mGrid, mStart, mGoal);

    mPath.construct(traversed, mStart, mGoal);
    drawPath();

    for (auto enemy : gameplayLayer->getCreepList()) {
        auto &enemyPath = enemy->getPath();
        auto from = enemyPath.getCurrentWaypoint().tile;

        enemyPath.construct(traversed, from, mGoal);
    }
}

void World::upgradeTower(cocos2d::Vec2 pTile) {
    auto tower = gameplayLayer->getTower(pTile);
    auto color = colors.at(tower->getLevel() + 1);

    tower->upgrade(color);
    mapLayer->setSlotColor(pTile, color);

    this->balanceTotalCoin(-tower->getCost());
}

bool World::spawnNextWave() {
    if (mCurrentWave < mWaves.size()) {
        auto wave = mWaves.at(mCurrentWave);

        for (unsigned int i = 0; i < wave.size(); i++) {
            gameplayLayer->addEnemy(wave.at(i), i, mPath);
        }

        mCurrentWave++;

        return true;
    }

    return false;
}

void World::buildScene() {
    backgroundLayer = LayerColor::create(Color4B(42, 45, 51, 255));

    // Prepare sample grid
    mGrid.create(Vec2(5, 10));
    for (int i = 0; i < mGrid.getSize().x; i++)
        mGrid.setNode(Vec2(i, 0), 2);
    mGrid.setNode(Vec2(2, 0), 0);

    mStart = Vec2(2, 9);
    mGoal = Vec2(2, 0);

    mapLayer = MapLayer::create(this);

    mPathCanvas = DrawNode::create();

    auto traversed = algorithm::traverse(mGrid, mStart, mGoal);
    if (mPath.isReached(traversed, mStart)) {
        mPath.construct(traversed, mStart, mGoal);
        drawPath();
    }

    gameplayLayer = GameplayLayer::create(this);
    gameplayLayer->addChild(mPathCanvas);

    hudLayer = HUDLayer::create(this);
    mWheelMenu.init(hudLayer, this);

    hudLayer->notify('I', "Game is starting!", 2.f);

    this->addChild(backgroundLayer);
    this->addChild(mapLayer);
    this->addChild(gameplayLayer);
    this->addChild(hudLayer);

    this->scheduleUpdate();
}

void World::connectListeners() {
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch *pTouch, Event *pEvent) {
        Vec2 touched = Vec2(-1, -1);
        Vec2 size = mGrid.getSize();
        for (int i = 0; i < size.x; i++) {
            for (int j = 0; j < size.y; j++) {
                Vec2 current = Vec2(i, j);
                Vec2 location = algorithm::toCircularGrid(current);
                Rect boundingBox = Rect(location.x - NODE_TOUCH_SIZE / 2.f,
                                        location.y - NODE_TOUCH_SIZE / 2.f,
                                        NODE_TOUCH_SIZE, NODE_TOUCH_SIZE);

                if (boundingBox.containsPoint(pTouch->getLocation())) {
                    touched = current;
                    break;
                }
            }
        }

        if (mWheelMenu.isOpen()) {
            mWheelMenu.close();
        } else if (touched.x > -1 && touched.y > -1) {
            mWheelMenu.openAt(touched);
        }

        return true;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool World::isAvailable(const TraverseData &pTraversed, cocos2d::Vec2 pTile) {
    if (!mPath.isReached(pTraversed, mStart)) {
        hudLayer->notify('E', "You can't block the path!");
        return false;
    }

    for (auto enemy : gameplayLayer->getCreepList()) {
        auto current = enemy->getPath().getCurrentWaypoint().tile;

        if ((current == pTile) || !enemy->getPath().isReached(pTraversed, current)) {
            hudLayer->notify('E', "You can't block enemies!");
            return false;
        }
    }

    return true;
}

void World::drawPath() {
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
