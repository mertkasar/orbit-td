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
#include <platform/CCFileUtils.h>

USING_NS_CC;

#define STARTING_COIN 500
#define STARTING_LIFE 10

std::map<unsigned int, TowerModel> towerModels;

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
    towerModels.insert(std::make_pair(ModelID::TURRET,
                                      TowerModel{ModelID::TURRET, "turret_gun.png", 10, 150.f, 3.f, 0.3f}));
    towerModels.insert(std::make_pair(ModelID::LASER,
                                      TowerModel{ModelID::LASER, "laser_gun.png", 30, 150.f, 0.3f, 0.f}));
    towerModels.insert(std::make_pair(ModelID::R_LAUNCHER,
                                      TowerModel{ModelID::R_LAUNCHER, "r_launcher.png", 50, 150.f, 30.f, 1.f}));

    colors.push_back(Color::GREEN);
    colors.push_back(Color::YELLOW);
    colors.push_back(Color::BLUE);

    audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
    //loadResources();
    loadModel("models/raptor.plist");
    loadModel("models/speedy.plist");
    loadModel("models/pulsar.plist");
    loadModel("models/panzer.plist");
    buildScene();
    connectListeners();

    //Init waves
    mWaves.clear();
    mWaves.push_back(std::vector<ModelID>{SPEEDY, RAPTOR, PULSAR, PANZER});
    mWaves.push_back(std::vector<ModelID>{RAPTOR});
    mWaves.push_back(std::vector<ModelID>{RAPTOR, RAPTOR, RAPTOR});
    mWaves.push_back(std::vector<ModelID>{SPEEDY, SPEEDY, RAPTOR, RAPTOR, RAPTOR});
    mWaves.push_back(std::vector<ModelID>{RAPTOR, RAPTOR, PULSAR, PULSAR});
    mWaves.push_back(std::vector<ModelID>{RAPTOR, RAPTOR, RAPTOR, SPEEDY, SPEEDY, PULSAR, PANZER});
    mWaves.push_back(std::vector<ModelID>(7, SPEEDY));
    mWaves.push_back(std::vector<ModelID>{PULSAR, PULSAR, PULSAR, PULSAR, PULSAR, PULSAR, PANZER});
    mWaves.push_back(std::vector<ModelID>{10, PULSAR});
    mWaves.push_back(std::vector<ModelID>{SPEEDY, SPEEDY, SPEEDY, SPEEDY, RAPTOR, RAPTOR, RAPTOR, RAPTOR, PULSAR,
                                          PULSAR, PANZER, PANZER});
    mWaves.push_back(std::vector<ModelID>{15, PANZER});

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

bool World::placeTower(ModelID pType, Vec2 pTile) {
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
            const auto &model = getModel(wave.at(i));
            gameplayLayer->addEnemy(model, i, mapLayer->mPath);
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

void World::loadModel(std::string pPath) {
    auto data = FileUtils::getInstance()->getValueMapFromFile(pPath);
    auto key = (unsigned int) data.at("id").asInt();

    mModels.insert(std::make_pair(key, data));
}
