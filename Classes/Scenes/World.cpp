#include <Scenes/World.h>

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
#include <SimpleAudioEngine.h>

#include <Scenes/MapLayer.h>
#include <Scenes/GameplayLayer.h>
#include <Scenes/HUDLayer.h>
#include <Entities/Towers/Tower.h>
#include <Entities/Creep.h>
#include <Entities/WheelMenu.h>

#include <sstream>

USING_NS_CC;

#define STARTING_COIN 500
#define STARTING_LIFE 10

std::map<unsigned int, TowerModel> models;

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
//    physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

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

    // Load tower models
    models.insert(std::make_pair(TowerTypes::TURRET,
                                 TowerModel{TowerTypes::TURRET, "turret_gun.png", 10, 150.f, 3.f, 0.3f}));
    models.insert(std::make_pair(TowerTypes::LASER,
                                 TowerModel{TowerTypes::LASER, "laser_gun.png", 30, 150.f, 0.3f, 0.f}));
    models.insert(std::make_pair(TowerTypes::R_LAUNCHER,
                                 TowerModel{TowerTypes::R_LAUNCHER, "r_launcher.png", 50, 150.f, 30.f, 1.f}));

    colors.push_back(Color::GREEN);
    colors.push_back(Color::YELLOW);
    colors.push_back(Color::BLUE);

    audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
    //loadResources();
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

    /*audioEngine->setBackgroundMusicVolume(0.6f);
    audioEngine->playBackgroundMusic("audio/ambient.mp3", true);*/

    audioEngine->setBackgroundMusicVolume(0.f);
    audioEngine->setEffectsVolume(0.f);

    return true;
}

void World::update(float pDelta) {
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
    auto traversed = mapLayer->traverseAgainst(pTile, 1);

    if (!mapLayer->isPathClear(traversed)) {
        hudLayer->notify('E', "You can't block the path!");
        return false;
    };

    if (!gameplayLayer->isEnemyPathsClear(traversed, pTile)) {
        hudLayer->notify('E', "You can't block enemies!");
        return false;
    };

    gameplayLayer->addTower(pType, pTile);
    mapLayer->activateSlot(pTile);

    mapLayer->updateMap(traversed, pTile, 1);
    gameplayLayer->updateEnemyPaths(traversed, mapLayer->mGoal);

    return true;
}

void World::destroyTower(Vec2 pTile) {
    auto traversed = mapLayer->traverseAgainst(pTile, 0);

    gameplayLayer->deleteTower(pTile);
    mapLayer->deactivateSlot(pTile);

    mapLayer->updateMap(traversed, pTile, 0);
    gameplayLayer->updateEnemyPaths(traversed, mapLayer->mGoal);
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
            gameplayLayer->addEnemy(wave.at(i), i, mapLayer->mPath);
        }

        mCurrentWave++;

        return true;
    }

    return false;
}

void World::buildScene() {
    backgroundLayer = LayerColor::create(Color4B(Color::BG));

    auto gameCanvas = Node::create();

    mapLayer = MapLayer::create(this);
    gameCanvas->addChild(mapLayer);

    gameplayLayer = GameplayLayer::create(this);
    gameCanvas->addChild(gameplayLayer);

    hudLayer = HUDLayer::create(this);
    mWheelMenu = WheelMenu::create(this);

    hudLayer->notify('I', "Game is starting!", 2.f);

    this->addChild(backgroundLayer);
    this->addChild(gameCanvas);
    this->addChild(hudLayer);
    this->addChild(mWheelMenu);

    this->scheduleUpdate();
}

void World::connectListeners() {
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch *pTouch, Event *pEvent) {
        Vec2 touched = mapLayer->getTouchedSlot(pTouch->getLocation());

        if (mWheelMenu->isOpen()) {
            mWheelMenu->close();
        } else if (touched.x > -1 && touched.y > -1) {
            mWheelMenu->openAt(touched);
        }

        return true;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void World::loadResources() {
    audioEngine->preloadBackgroundMusic("audio/ambient.mp3");
    audioEngine->preloadEffect("audio/explosion_1.wav");
    audioEngine->preloadEffect("audio/explosion_2.wav");
    audioEngine->preloadEffect("audio/explosion_3.wav");
    audioEngine->preloadEffect("audio/laser_gun.wav");
    audioEngine->preloadEffect("audio/missile_launch.wav");
    audioEngine->preloadEffect("audio/click.wav");
    audioEngine->preloadEffect("audio/open.wav");
    audioEngine->preloadEffect("audio/deploy.wav");
    audioEngine->preloadEffect("audio/upgrade.wav");
    audioEngine->preloadEffect("audio/buzz.wav");
}
