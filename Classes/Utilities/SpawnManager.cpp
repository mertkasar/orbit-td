#include "SpawnManager.h"

#include "../Scenes/World.h"
#include "../Scenes/GameplayLayer.h"
#include "../Scenes/HUDLayer.h"
#include "../Scenes/MapLayer.h"
#include "../Entities/ResultPanel.h"

#include <platform/CCFileUtils.h>
#include <2d/CCActionInstant.h>

#define START_DELAY 5

USING_NS_CC;

SpawnManager::SpawnManager(World *world) {
    _world = world;
    CCLOG("SpawnManager created");
}

SpawnManager::~SpawnManager() {
    CCLOG("SpawnManager deleted");
}

SpawnManager *SpawnManager::create(World *world) {
    SpawnManager *layer = new(std::nothrow) SpawnManager(world);

    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    } else {
        delete layer;
        layer = NULL;
        return NULL;
    }
}

bool SpawnManager::init() {
    if (!Node::init())
        return false;

    _waves = FileUtils::getInstance()->getValueVectorFromFile("waves.plist");
    reset();

    return true;
}


void SpawnManager::update(float delta) {
    if (_world->_gameplayLayer->getEnemyShips().size() <= 0) {
        if (!_spawned) {
            if (!isCleared()) {
                scheduleOnce([&](float) { _world->_mapLayer->drawPath(); }, 1.f, "draw_path");
                scheduleOnce(CC_SCHEDULE_SELECTOR(SpawnManager::spawnNextWave), 5.f);
                _spawned = true;
            } else {
                _world->_hudLayer->notify('I', "All waves are cleared!");
                auto resultPanel = ResultPanel::create(_world);
                resultPanel->runAction(resultPanel->show());
                _world->addChild(resultPanel);
                unscheduleUpdate();
            }
        }
    }
}

void SpawnManager::spawnNextWave(float delta) {
    auto wave = _waves.at(_currentWave).asValueVector();

    _world->spawnWave(wave);

    _currentWave++;
    _spawned = false;
}

void SpawnManager::reset() {
    _currentWave = 0;
    _spawned = false;

    scheduleOnce([&](float delta) { scheduleUpdate(); }, START_DELAY, "start");
}
