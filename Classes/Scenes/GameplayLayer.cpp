#include <Scenes/GameplayLayer.h>

#include <base/CCDirector.h>
#include <physics/CCPhysicsContact.h>
#include <base/CCEventDispatcher.h>
#include <physics/CCPhysicsWorld.h>
#include <physics/CCPhysicsContact.h>

#include "World.h"
#include <Utilities/Algorithm.h>
#include <Entities/Creep.h>
#include <Entities/Towers/Turret.h>
#include <Entities/Towers/Laser.h>
#include <Entities/Towers/RLauncher.h>

USING_NS_CC;

GameplayLayer::GameplayLayer(World *pWorld) {
    mWorld = pWorld;
}

GameplayLayer *GameplayLayer::create(World *pWorld) {
    GameplayLayer *layer = new(std::nothrow) GameplayLayer(pWorld);

    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    } else {
        delete layer;
        layer = NULL;
        return NULL;
    }
}

bool GameplayLayer::init() {
    if (!Layer::init())
        return false;

    this->setName("gameplay_layer");

    this->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

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

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    this->scheduleUpdate();

    return true;
}

void GameplayLayer::update(float pDelta) {
    //Clear dead enemy objects
    for (auto enemy : mCreeps)
        if (enemy->isDead()) {
            enemy->removeFromParent();
            mCreeps.eraseObject(enemy);

            if (enemy->isKilled())
                mWorld->balanceTotalCoin(enemy->getReward());
            else if (enemy->isReachedEnd())
                mWorld->balanceRemainingLife(-1);
        }
}

void GameplayLayer::addEnemy(CreepTypes pType, int pOrder, Path &pPath) {
    auto enemy = mCreepPool.fetch();

    Vec2 spawnPosition = Vec2(1230, 360.f) + Vec2(pOrder * 100, 0);
    enemy->ignite(pType, spawnPosition, pPath);

    this->addChild(enemy);
    mCreeps.pushBack(enemy);
}


void GameplayLayer::createMock(TowerTypes pType, cocos2d::Vec2 pTile) {
    mMock = nullptr;

    switch (pType) {
        case TURRET:
            mMock = Turret::create();
            break;
        case LASER:
            mMock = Laser::create();
            break;
        case R_LAUNCHER:
            mMock = RLauncher::create();
            break;
        default:
            break;
    }

    if (mMock) {
        auto position = algorithm::toCircularGrid(pTile);
        mMock->setPosition(position);
        mMock->setVerbose(true);

        this->addChild(mMock);
    }
}

void GameplayLayer::buildMock(Vec2 pTile) {
    mWorld->balanceTotalCoin(-mMock->getCost());

    mMock->build();
    mTowerMap.insert(std::make_pair(pTile, mMock));
}

void GameplayLayer::removeMock() {
    if (mMock) {
        mMock->removeFromParentAndCleanup(true);
        mMock = nullptr;
    }
}

Tower *GameplayLayer::getTower(Vec2 pTile) {
    auto found = mTowerMap.find(pTile);

    assert(found != mTowerMap.end());

    return found->second;
}

void GameplayLayer::deleteTower(Vec2 pTile) {
    auto found = mTowerMap.find(pTile);

    assert(found != mTowerMap.end());

    auto tower = found->second;
    mWorld->balanceTotalCoin(tower->getCost());
    tower->removeFromParentAndCleanup(true);
    mTowerMap.erase(found);
}
