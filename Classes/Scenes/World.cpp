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
#include "../Utilities/SpawnManager.h"

#include <base/CCDirector.h>
#include <base/CCEventDispatcher.h>
#include <base/CCEventListenerTouch.h>
#include <base/CCUserDefault.h>
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

    _currentState = IDLE;

    _prefs = UserDefault::getInstance();
    _audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
    loadResources();

    _backgroundSprite = Sprite::create("textures/background.png");
    _backgroundSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _backgroundSprite->setPosition(Vec2(0.f, -80.f));
    addChild(_backgroundSprite);

    _planet = Planet::create();
    addChild(_planet);

    setState(GAMEPLAY);

    auto muted = _prefs->getBoolForKey("muted");
    _audioEngine->playBackgroundMusic("audio/ambient.mp3", true);

    if (!muted) {
        _audioEngine->setBackgroundMusicVolume(0.6f);
    } else {
        _audioEngine->setBackgroundMusicVolume(0.f);
        _audioEngine->setEffectsVolume(0.f);
    }

    return true;
}

void World::resetGame() {
    if (_currentState == GAMEPLAY) {
        _mapLayer->reset();

        _gameplayLayer->reset();
        _gameplayLayer->resumeScene();

        _hudLayer->updateLife();

        _spawnManager->reset();
    } else
        CCLOG("Failed to reset the game: Wrong state!");
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

    if (_gameplayLayer->getEnemiesPresent() == 0)
        _mapLayer->drawPath();

    return true;
}

void World::destroyTower(Vec2 tile) {
    auto traversed = _mapLayer->traverseAgainst(tile, 0);

    _gameplayLayer->deleteTower(tile);
    _mapLayer->deactivateSlot(tile);

    _mapLayer->updateMap(traversed, tile, 0);
    _gameplayLayer->updateEnemyPaths(traversed, _mapLayer->_goal);

    if (_gameplayLayer->getEnemiesPresent() == 0)
        _mapLayer->drawPath();
}

void World::upgradeTower(cocos2d::Vec2 tile) {
    auto tower = _gameplayLayer->getTower(tile);

    tower->upgrade();
    _mapLayer->setSlotColor(tile, tower->getBaseColor());

    _gameplayLayer->balanceTotalEnergy(-tower->getCost());
}

void World::spawnWave(const cocos2d::ValueVector &waveData) {
    for (unsigned int i = 0; i < waveData.size(); i++) {
        const auto &model = getModel((unsigned int) waveData.at(i).asInt());
        _gameplayLayer->addEnemyShip(model, i, _mapLayer->_path, _spawnManager->getCurrentWave());
    }

    _hudLayer->updateWaveIndicators(_spawnManager->getCurrentWave(), _spawnManager->getMaxWave());
}

void World::endGame(bool win) {
    if (win)
        _hudLayer->notify('I', "All waves are cleared!");
    else
        _hudLayer->notify('I', "Game Over!");

    auto resultPanel = ResultPanel::create(this, win, _gameplayLayer->getTotalPoint(), _gameplayLayer->getTotalEnergy(),
                                           _gameplayLayer->getRemainingLife());
    resultPanel->runAction(resultPanel->show());
    addChild(resultPanel);
}

void World::setState(World::State state) {
    if (state == MAIN_MENU) {
        _mainMenuLayer = MainMenuLayer::create(this);

        if (_currentState == GAMEPLAY) {
            _planet->runAction(EaseExponentialIn::create(MoveBy::create(2.5f, Vec2(450.f, -360.f))));
            _backgroundSprite->runAction(EaseExponentialIn::create(MoveBy::create(2.5f, Vec2(-40.f, -80.f))));

            _gameplayLayer->close();
            _spawnManager->removeFromParent();
            _mapLayer->close();
            _hudLayer->close();

            if (_wheelMenu->isOpen())
                _wheelMenu->close();

            _mainMenuLayer->show(2.5f);

            unscheduleUpdate();

            addChild(_mainMenuLayer);
        } else {
            _mainMenuLayer->show(0.f);
            addChild(_mainMenuLayer);
        }

        _currentState = state;
    } else if (state == GAMEPLAY) {
        if (_currentState == MAIN_MENU) {
            _mainMenuLayer->close();
        }

        _planet->runAction(EaseExponentialIn::create(MoveBy::create(2.5f, Vec2(-450.f, 360.f))));
        _backgroundSprite->runAction(EaseExponentialIn::create(MoveBy::create(2.5f, Vec2(40.f, 80.f))));

        auto gameCanvas = Node::create();

        _mapLayer = MapLayer::create(this);
        gameCanvas->addChild(_mapLayer);

        _gameplayLayer = GameplayLayer::create(this);
        gameCanvas->addChild(_gameplayLayer);

        addChild(gameCanvas);

        _spawnManager = SpawnManager::create(this);
        addChild(_spawnManager);

        _hudLayer = HUDLayer::create(this);
        _hudLayer->show(2.5f);
        _hudLayer->updateWaveIndicators(_spawnManager->getCurrentWave(), _spawnManager->getMaxWave());
        addChild(_hudLayer);

        _wheelMenu = WheelMenu::create(this);
        addChild(_wheelMenu);

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
