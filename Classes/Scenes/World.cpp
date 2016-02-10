#include "World.h"

#include "MapLayer.h"
#include "GameplayLayer.h"
#include "HUDLayer.h"
#include "MainMenuLayer.h"
#include "../Entities/Turret.h"
#include "../Entities/EnemyShip.h"
#include "../Entities/WheelMenu.h"
#include "../Entities/Planet.h"
#include "../Entities/ResultPanel.h"

#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <base/CCEventListenerTouch.h>
#include <2d/CCSpriteFrameCache.h>
#include <2d/CCActionInterval.h>
#include <2d/CCActionInstant.h>
#include <2d/CCActionEase.h>
#include <2d/CCSprite.h>
#include <physics/CCPhysicsWorld.h>
#include <physics/CCPhysicsContact.h>
#include <ui/UIImageView.h>
#include <SimpleAudioEngine.h>
#include <platform/CCFileUtils.h>

#include <sstream>

#define STARTING_COIN 1000
#define STARTING_LIFE 20
#define START_DELAY 5

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
//  physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = World::create();
    layer->setPhysicsWorld(physicsWorld);

    scene->addChild(layer);

    return scene;
}

bool World::init() {
    if (!Layer::init()) {
        return false;
    }

    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    _canvasCenter = Vec2(_visibleSize / 2.f) + _origin;

    _totalCoin = STARTING_COIN;
    _life = STARTING_LIFE;

    _audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
    loadResources();

    _backgroundSprite = Sprite::create("textures/background.png");
    _backgroundSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _backgroundSprite->setPosition(Vec2(0.f, -80.f));
    addChild(_backgroundSprite);

    _planet = Planet::create();
    addChild(_planet);

    setState(MAIN_MENU);

    _waves = FileUtils::getInstance()->getValueVectorFromFile("waves.plist");
    _currentWave = 0;
    _cleared = false;

    /*_audioEngine->setBackgroundMusicVolume(0.6f);
    _audioEngine->playBackgroundMusic("audio/ambient.mp3", true);*/

    _audioEngine->setBackgroundMusicVolume(0.f);
    _audioEngine->setEffectsVolume(0.f);

    return true;
}

void World::update(float delta) {
    if (_gameplayLayer->getCreepList().size() <= 0) {
        if (!spawnNextWave())
            _cleared = true;
    }

    if (_life <= 0) {
        _hudLayer->notify('I', "Game Over!");
        _gameplayLayer->pauseScene();
        auto resultPanel = ResultPanel::create(this);
        resultPanel->runAction(resultPanel->show());
        addChild(resultPanel);
        unscheduleUpdate();
    }

    if (isCleared()) {
        _hudLayer->notify('I', "All waves are cleared!");
        auto resultPanel = ResultPanel::create(this);
        resultPanel->runAction(resultPanel->show());
        addChild(resultPanel);
        unscheduleUpdate();
    }
}

void World::resetGame() {
    if (_currentState == GAMEPLAY) {
        _mapLayer->reset();

        _gameplayLayer->reset();
        _gameplayLayer->resumeScene();

        _totalCoin = STARTING_COIN;
        _life = STARTING_LIFE;

        _hudLayer->updateLife();

        _currentWave = 0;
        _cleared = false;

        scheduleOnce([&](float delta){scheduleUpdate();}, START_DELAY, "start");
    } else CCLOG("Failed to reset the game: Wrong state!");
}

bool World::placeTower(ModelID type, Vec2 tile) {
    auto traversed = _mapLayer->traverseAgainst(tile, 1);

    if (!_mapLayer->isPathClear(traversed)) {
        _hudLayer->notify('E', "You can't block the path!");
        return false;
    };

    if (!_gameplayLayer->isEnemyPathsClear(traversed, tile)) {
        _hudLayer->notify('E', "You can't block enemies!");
        return false;
    };

    _gameplayLayer->addTower(type, tile);
    _mapLayer->activateSlot(tile);

    _mapLayer->updateMap(traversed, tile, 1);
    _gameplayLayer->updateEnemyPaths(traversed, _mapLayer->_goal);

    return true;
}

void World::destroyTower(Vec2 tile) {
    auto traversed = _mapLayer->traverseAgainst(tile, 0);

    _gameplayLayer->deleteTower(tile);
    _mapLayer->deactivateSlot(tile);

    _mapLayer->updateMap(traversed, tile, 0);
    _gameplayLayer->updateEnemyPaths(traversed, _mapLayer->_goal);
}

void World::upgradeTower(cocos2d::Vec2 tile) {
    auto tower = _gameplayLayer->getTower(tile);

    tower->upgrade();
    _mapLayer->setSlotColor(tile, tower->getBaseColor());

    balanceTotalCoin(-tower->getCost());
}

bool World::spawnNextWave() {
    if (_currentWave < _waves.size()) {
        auto wave = _waves.at(_currentWave).asValueVector();

        for (unsigned int i = 0; i < wave.size(); i++) {
            const auto &model = getModel((unsigned int) wave.at(i).asInt());
            _gameplayLayer->addEnemy(model, i, _mapLayer->_path);
        }

        _currentWave++;

        return true;
    }

    return false;
}

void World::setState(World::State state) {
    if (state == MAIN_MENU) {
        _mainMenuLayer = MainMenuLayer::create(this);

        if (_currentState == GAMEPLAY) {
            _planet->runAction(EaseExponentialIn::create(MoveBy::create(2.5f, Vec2(450.f, -360.f))));
            _backgroundSprite->runAction(EaseExponentialIn::create(MoveBy::create(2.5f, Vec2(-40.f, -80.f))));

            _gameplayLayer->close();
            _mapLayer->close();
            _hudLayer->close();

            _mainMenuLayer->show(2.5f);

            unscheduleUpdate();

            addChild(_mainMenuLayer);
        } else {
            _mainMenuLayer->show(0.f);
            addChild(_mainMenuLayer);
        }

        _currentState = state;
    } else if (state == GAMEPLAY) {
        _mainMenuLayer->close();

        _planet->runAction(EaseExponentialIn::create(MoveBy::create(2.5f, Vec2(-450.f, 360.f))));
        _backgroundSprite->runAction(EaseExponentialIn::create(MoveBy::create(2.5f, Vec2(40.f, 80.f))));

        auto gameCanvas = Node::create();

        _mapLayer = MapLayer::create(this);
        gameCanvas->addChild(_mapLayer);

        _gameplayLayer = GameplayLayer::create(this);
        gameCanvas->addChild(_gameplayLayer);

        addChild(gameCanvas);

        _hudLayer = HUDLayer::create(this);
        _hudLayer->notify('I', "Game is starting!", 2.f);
        _hudLayer->show(2.5f);
        addChild(_hudLayer);

        _wheelMenu = WheelMenu::create(this);
        addChild(_wheelMenu);

        scheduleOnce([&](float delta){scheduleUpdate();}, START_DELAY, "start");
        connectListeners();

        _currentState = state;
    }
}

void World::connectListeners() {
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch *touch, Event *pEvent) {
        Vec2 touched = _mapLayer->getTouchedSlot(touch->getLocation());

        if (_wheelMenu->isOpen()) {
            _wheelMenu->close();
        } else if (touched.x > -1 && touched.y > -1) {
            _wheelMenu->openAt(touched);
        }

        return true;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void World::loadResources() {
    auto index = FileUtils::getInstance()->getValueMapFromFile("index.plist");

    for (auto model : index.at("model").asValueVector()) {
        loadModel(model.asString());
    }

    /*auto audio = index.at("audio").asValueMap();
     for (auto bg : audio.at("background").asValueVector()) {
         _audioEngine->preloadBackgroundMusic(bg.asString().c_str());
     }

     for (auto effect : audio.at("effect").asValueVector()) {
         _audioEngine->preloadEffect(effect.asString().c_str());
     }*/

    auto spriteCache = SpriteFrameCache::getInstance();
    for (auto sheet : index.at("spritesheet").asValueVector()) {
        spriteCache->addSpriteFramesWithFile(sheet.asString());
    }
}

void World::loadModel(std::string path) {
    auto data = FileUtils::getInstance()->getValueMapFromFile(path);
    auto key = (unsigned int) data.at("id").asInt();

    _models.insert(std::make_pair(key, data));
}
