#include <Scenes/GameplayLayer.h>

#include <2d/CCParticleBatchNode.h>
#include <2d/CCParticleSystemQuad.h>
#include <base/CCDirector.h>
#include <physics/CCPhysicsContact.h>
#include <base/CCEventDispatcher.h>
#include <physics/CCPhysicsWorld.h>
#include <SimpleAudioEngine.h>

#include <Scenes/World.h>
#include <Scenes/MapLayer.h>
#include <Entities/Creep.h>
#include <Entities/Explosion.h>
#include <Entities/Bullet.h>
#include <Entities/Towers/Turret.h>
#include <Entities/Towers/Laser.h>
#include <Entities/Towers/RLauncher.h>
#include <Utilities/Shake.h>
#include <2d/CCSpriteFrameCache.h>
#include <sstream>

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

    mPaused = false;

    this->setName("gameplay_layer");

    auto spriteCache = SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile("textures/gameplay_layer.plist");

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

    mParticleBatch = ParticleBatchNode::create("textures/particles/missile_fire.png", 3000);
    this->addChild(mParticleBatch);

    this->scheduleUpdate();

    return true;
}

void GameplayLayer::update(float pDelta) {
    //Clear dead enemy objects
    for (auto enemy : mCreeps)
        if (enemy->isDead()) {
            enemy->removeFromParent();
            mCreeps.eraseObject(enemy);

            if (enemy->isKilled()) {
                addExplosion(enemy->getPosition(), 0.5f, 3.f);
                mWorld->balanceTotalCoin(enemy->getReward());
            } else if (enemy->isReachedEnd()) {
                mWorld->balanceRemainingLife(-1);
                mWorld->audioEngine->playEffect("audio/buzz.wav");
            }
        }
}

void GameplayLayer::addEnemy(const ValueMap &pModel, int pOrder, Path &pPath) {
    auto enemy = mCreepPool.fetch();

    Vec2 spawnPosition = Vec2(1230, 360.f) + Vec2(pOrder * 100, 0);
    enemy->ignite(pModel, spawnPosition, pPath);

    this->addChild(enemy);
    mCreeps.pushBack(enemy);
}

void GameplayLayer::addMissile(cocos2d::Vec2 pPosition, const cocos2d::Color3B &pBaseColor, float pDamage,
                               Creep *pTarget) {
    auto missile = mMissilePool.fetch();

    missile->ignite(pPosition, pBaseColor, pDamage, pTarget);

    auto emitter = missile->getEmitter();
    if (emitter->getParent() == nullptr) {
        mParticleBatch->addChild(emitter);
    }

    mWorld->audioEngine->playEffect("audio/missile_launch.wav");

    this->addChild(missile);
    mMissiles.pushBack(missile);
}

void GameplayLayer::addBullet(cocos2d::Vec2 pPosition, const cocos2d::Color3B &pBaseColor, float pDamage,
                              Creep *pTarget) {
    auto bullet = mBulletPool.fetch();

    bullet->ignite(pPosition, pBaseColor, pDamage, pTarget);

    mWorld->audioEngine->playEffect("audio/laser_gun.wav", false, 1.0f, 0.0f, 0.3f);

    this->addChild(bullet);
}

void GameplayLayer::addExplosion(cocos2d::Vec2 pPosition, float pDuration, float pStrength) {
    // Create explosion animation
    auto explosion = mExplosionPool.fetch();
    explosion->ignite(pPosition);
    this->addChild(explosion);

    // Create shake animation
    this->getParent()->runAction(Shake::actionWithDuration(pDuration, pStrength));

    // Play a random explosion sfx
    int index = random(1, 3);
    std::stringstream ss;
    ss << "audio/explosion_" << index << ".wav";
    mWorld->audioEngine->playEffect(ss.str().c_str());
}

void GameplayLayer::addTower(ModelID pType, cocos2d::Vec2 pTile) {
    Tower *tower = nullptr;

    switch (pType) {
        case TURRET:
            tower = Turret::create();
            break;
        case LASER:
            tower = Laser::create();
            break;
        case R_LAUNCHER:
            tower = RLauncher::create();
            break;
        default:
            break;
    }

    if (tower) {
        auto position = algorithm::toCircularGrid(pTile);
        tower->setPosition(position);

        mWorld->balanceTotalCoin(-tower->getCost());
        mTowerMap.insert(std::make_pair(pTile, tower));

        this->addChild(tower);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/deploy.wav");
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

void GameplayLayer::pauseScene() {
    this->pause();

    for (auto child : this->getChildren())
        child->pause();

    mWorld->getPhysicsWorld()->setSpeed(0.f);

    for (auto emitter : mParticleBatch->getChildren())
        emitter->pause();

    mPaused = true;
}

void GameplayLayer::resumeScene() {
    this->resume();

    for (auto child : this->getChildren())
        child->resume();

    mWorld->getPhysicsWorld()->setSpeed(1.f);

    for (auto emitter : mParticleBatch->getChildren())
        emitter->resume();

    mPaused = false;
}

bool GameplayLayer::isEnemyPathsClear(const TraverseData &pTraversed, Vec2 pTile) {
    for (auto enemy : mCreeps) {
        auto current = enemy->getPath().getCurrentWaypoint().tile;

        if ((current == pTile) || !enemy->getPath().isReached(pTraversed, current))
            return false;
    }

    return true;
}

void GameplayLayer::updateEnemyPaths(TraverseData pTraversed, Vec2 pGoal) {
    for (auto enemy : mCreeps) {
        auto &enemyPath = enemy->getPath();
        auto from = enemyPath.getCurrentWaypoint().tile;

        enemyPath.construct(pTraversed, from, pGoal);
    }
}
