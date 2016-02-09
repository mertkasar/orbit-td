#include "GameplayLayer.h"

#include "World.h"
#include "MapLayer.h"
#include "HUDLayer.h"
#include "../Entities/EnemyShip.h"
#include "../Entities/Explosion.h"
#include "../Entities/Bullet.h"
#include "../Entities/MachineGun.h"
#include "../Entities/LaserGun.h"
#include "../Entities/MissileLauncher.h"
#include "../Utilities/Shake.h"

#include <2d/CCParticleBatchNode.h>
#include <2d/CCParticleSystemQuad.h>
#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <physics/CCPhysicsContact.h>
#include <physics/CCPhysicsWorld.h>
#include <SimpleAudioEngine.h>

#include <sstream>

USING_NS_CC;

GameplayLayer::GameplayLayer(World *world) {
    _world = world;
}

GameplayLayer *GameplayLayer::create(World *world) {
    GameplayLayer *layer = new(std::nothrow) GameplayLayer(world);

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

    _paused = false;

    setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [](PhysicsContact &pContact) {
        auto a = pContact.getShapeA()->getBody();
        auto b = pContact.getShapeB()->getBody();

        if ((a->getCategoryBitmask() == TOWER_RANGE_MASK && b->getCategoryBitmask() == ENEMY_MASK) ||
            (a->getCategoryBitmask() == ENEMY_MASK && b->getCategoryBitmask() == TOWER_RANGE_MASK)) {

            Turret *tower = nullptr;
            EnemyShip *enemy = nullptr;

            if (a->getCategoryBitmask() == TOWER_RANGE_MASK) {
                tower = static_cast<Turret *>(a->getNode());
                enemy = static_cast<EnemyShip *>(b->getNode());
            } else {
                tower = static_cast<Turret *>(b->getNode());
                enemy = static_cast<EnemyShip *>(a->getNode());
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

            Turret *tower = nullptr;
            EnemyShip *enemy = nullptr;

            if (a->getCategoryBitmask() == TOWER_RANGE_MASK) {
                tower = static_cast<Turret *>(a->getNode());
                enemy = static_cast<EnemyShip *>(b->getNode());
            } else {
                tower = static_cast<Turret *>(b->getNode());
                enemy = static_cast<EnemyShip *>(a->getNode());
            }

            tower->removeTarget(enemy);
        }
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    _particleBatch = ParticleBatchNode::create("textures/particles/missile_fire.png", 3000);
    addChild(_particleBatch);

    scheduleUpdate();
    _world->scheduleUpdate();

    return true;
}

void GameplayLayer::update(float delta) {
    //Clear dead enemy objects
    for (auto enemy : _creeps)
        if (enemy->isDead()) {
            enemy->removeFromParent();
            _creeps.eraseObject(enemy);

            if (enemy->isKilled()) {
                addExplosion(enemy->getPosition(), 0.5f, 3.f);
                _world->balanceTotalCoin(enemy->getReward());
            } else if (enemy->isReachedEnd()) {
                _world->balanceRemainingLife(-1);
                _world->_hudLayer->updateLife();
                _world->_audioEngine->playEffect("audio/buzz.wav");
            }
        }
}

void GameplayLayer::reset() {
    for (auto creep : _creeps)
        creep->removeFromParent();
    _creeps.clear();

    for (auto missile : _missiles)
        missile->removeFromParent();
    _missiles.clear();

    for (auto pair : _towerMap)
        pair.second->removeFromParent();
    _towerMap.clear();

    for (auto bullet : _bullets)
        bullet->removeFromParent();
    _bullets.clear();

    _particleBatch->removeAllChildren();
}

void GameplayLayer::addEnemy(const ValueMap &model, int order, Path &path) {
    auto enemy = _creepPool.fetch();

    Vec2 spawnPosition = Vec2(1230, 360.f) + Vec2(order * 100, 0);
    enemy->restart(model, spawnPosition, path);

    addChild(enemy);
    _creeps.pushBack(enemy);
}

void GameplayLayer::addMissile(cocos2d::Vec2 position, const cocos2d::Color3B &baseColor, float damage,
                               EnemyShip *target) {
    auto missile = _missilePool.fetch();

    missile->restart(position, baseColor, damage, target);

    auto emitter = missile->getEmitter();
    if (emitter->getParent() == nullptr) {
        _particleBatch->addChild(emitter);
    }

    _world->_audioEngine->playEffect("audio/missile_launch.wav");

    addChild(missile);
    _missiles.pushBack(missile);
}

void GameplayLayer::addBullet(cocos2d::Vec2 position, const cocos2d::Color3B &baseColor, float damage,
                              EnemyShip *target) {
    auto bullet = _bulletPool.fetch();

    bullet->restart(position, baseColor, damage, target);

    _world->_audioEngine->playEffect("audio/machine_gun.wav", false, 1.0f, 0.0f, 0.3f);

    addChild(bullet);
    _bullets.pushBack(bullet);
}

void GameplayLayer::addExplosion(cocos2d::Vec2 position, float duration, float strength) {
    // Create explosion animation
    auto explosion = _explosionPool.fetch();
    explosion->restart(position);
    addChild(explosion);

    // Create shake animation
    getParent()->runAction(Shake::actionWithDuration(duration, strength));

    // Play a random explosion sfx
    int index = random(1, 3);
    std::stringstream ss;
    ss << "audio/explosion_" << index << ".wav";
    _world->_audioEngine->playEffect(ss.str().c_str());
}

void GameplayLayer::addTower(ModelID type, cocos2d::Vec2 tile) {
    Turret *tower = nullptr;

    switch (type) {
        case MACHINE_GUN:
            tower = MachineGun::create(_world->getModel(type));
            break;
        case LASER_GUN:
            tower = LaserGun::create(_world->getModel(type));
            break;
        case MISSILE_LAUNCHER:
            tower = MissileLauncher::create(_world->getModel(type));
            break;
        default:
            break;
    }

    if (tower) {
        auto position = algorithm::toCircularGrid(tile);
        tower->setPosition(position);

        _world->balanceTotalCoin(-tower->getCost());
        _towerMap.insert(std::make_pair(tile, tower));

        addChild(tower);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/deploy.wav");
    }
}

Turret *GameplayLayer::getTower(Vec2 tile) {
    auto found = _towerMap.find(tile);

    assert(found != _towerMap.end());

    return found->second;
}

void GameplayLayer::deleteTower(Vec2 tile) {
    auto found = _towerMap.find(tile);

    assert(found != _towerMap.end());

    auto tower = found->second;
    _world->balanceTotalCoin(tower->getCost());
    tower->removeFromParentAndCleanup(true);
    _towerMap.erase(found);
}

void GameplayLayer::pauseScene() {
    pause();

    for (auto child : getChildren())
        child->pause();

    _world->getPhysicsWorld()->setSpeed(0.f);

    for (auto emitter : _particleBatch->getChildren())
        emitter->pause();

    _paused = true;
}

void GameplayLayer::resumeScene() {
    resume();

    for (auto child : getChildren())
        child->resume();

    _world->getPhysicsWorld()->setSpeed(1.f);

    for (auto emitter : _particleBatch->getChildren())
        emitter->resume();

    _paused = false;
}

bool GameplayLayer::isEnemyPathsClear(const TraverseData &traversed, Vec2 node) {
    for (auto enemy : _creeps) {
        auto current = enemy->getPath().getCurrentWaypoint()._tile;

        if ((current == node) || !enemy->getPath().isReached(traversed, current))
            return false;
    }

    return true;
}

void GameplayLayer::updateEnemyPaths(const TraverseData &traversed, Vec2 goal) {
    for (auto enemy : _creeps) {
        auto &enemyPath = enemy->getPath();
        auto from = enemyPath.getCurrentWaypoint()._tile;

        enemyPath.construct(traversed, from, goal);
    }
}
