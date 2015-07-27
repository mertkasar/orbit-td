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

    placeTower(Vec2(0, 8));
    placeTower(Vec2(1, 8));
    placeTower(Vec2(2, 8));
    placeTower(Vec2(3, 8));
    placeTower(Vec2(4, 6));
    placeTower(Vec2(3, 6));
    placeTower(Vec2(2, 6));
    placeTower(Vec2(1, 6));
    placeTower(Vec2(0, 4));
    placeTower(Vec2(1, 4));

    constructPath(Vec2(2, 9), Vec2(2, 0));

    auto path = DrawNode::create();
    auto waypoints = mPath.getWaypointList();
    for (unsigned int i = 0; i < waypoints.size(); i++) {
        auto currentWaypoint = waypoints.at(i);

        path->drawSolidCircle(currentWaypoint.first, 6.f, 0.f, 50, Color4F::RED);
        path->drawCircle(currentWaypoint.first, currentWaypoint.second, 0.f, 50, false, Color4F::RED);

        if (i != 0) {
            auto previousWaypoint = waypoints.at(i - 1);
            path->drawLine(currentWaypoint.first, previousWaypoint.first, Color4F::RED);
        }
    }

    mGameplayLayer->addChild(path);

    this->addChild(mBackgroundLayer);
    this->addChild(mGameplayLayer);

    spawnEnemy(0.f);
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::spawnEnemy), 2.f);
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

void GameScene::constructPath(Vec2 pStart, Vec2 pGoal) {
    auto traversed = algorithm::traverse(mGrid, pStart, pGoal);
    std::vector<Vec2> waypoints;

    bool isInside = false;
    for (auto pair : traversed) {
        Vec2 loc = pair.second;

        if (loc.x == pGoal.x && loc.y == pGoal.y) {
            isInside = true;
            break;
        }
    }

    if (isInside) {
        auto current = pGoal;

        while (current != pStart) {
            for (auto pair : traversed) {
                if (current == pair.second) {
                    current = pair.first;

                    waypoints.push_back(pair.second);
                    break;
                }
            }
        }

        waypoints.push_back(pStart);

        std::reverse(waypoints.begin(), waypoints.end());

        for (auto waypoint : waypoints) {
            CCLOG("%i, %i", (int) waypoint.x, (int) waypoint.y);
            mPath.addWaypoint(algorithm::toCircularGrid(waypoint));
        }
    } else
        CCLOG("Path couldn't constructed!");
}