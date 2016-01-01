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

    mAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
    loadResources();

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
                createExplosion(enemy->getPosition(), 0.5f, 3.f);
                mWorld->balanceTotalCoin(enemy->getReward());
            } else if (enemy->isReachedEnd())
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

void GameplayLayer::addMissile(cocos2d::Vec2 pPosition, const cocos2d::Color3B &pBaseColor, float pDamage,
                               Creep *pTarget) {
    auto missile = mMissilePool.fetch();

    missile->ignite(pPosition, pBaseColor, pDamage, pTarget);

    auto emitter = missile->getEmitter();
    if (emitter->getParent() == nullptr) {
        mParticleBatch->addChild(emitter);
    }

    this->addChild(missile);
    mMissiles.pushBack(missile);
}

void GameplayLayer::addBullet(cocos2d::Vec2 pPosition, const cocos2d::Color3B &pBaseColor, float pDamage,
                              Creep *pTarget) {
    auto bullet = mBulletPool.fetch();

    bullet->ignite(pPosition, pBaseColor, pDamage, pTarget);

    this->addChild(bullet);
}

void GameplayLayer::addExplosion(cocos2d::Vec2 pPosition) {
    auto explosion = mExplosionPool.fetch();

    explosion->ignite(pPosition);

    this->addChild(explosion);
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

void GameplayLayer::loadResources() {
    auto spriteCache = SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile("textures/gameplay_layer.plist");

    mAudioEngine->preloadBackgroundMusic("audio/ambience.mp3");
    mAudioEngine->preloadEffect("audio/explosion_1.wav");
    mAudioEngine->preloadEffect("audio/explosion_2.wav");
    mAudioEngine->preloadEffect("audio/explosion_3.wav");
    mAudioEngine->playBackgroundMusic("audio/ambience.mp3", true);
}

void GameplayLayer::createExplosion(Vec2 pPosition, float pDuration, float pStrength) {
    addExplosion(pPosition);
    this->getParent()->runAction(Shake::actionWithDuration(pDuration, pStrength));

    // Get a random explosion effect
    int index = random(1, 3);
    std::stringstream ss;
    ss << "audio/explosion_" << index << ".wav";
    mAudioEngine->playEffect(ss.str().c_str());
}
