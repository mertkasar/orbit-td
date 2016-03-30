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

    //Load wave patterns from file
    _patterns = FileUtils::getInstance()->getValueVectorFromFile("waves.plist");

    reset();

    return true;
}


void SpawnManager::update(float delta) {
    if (_world->_gameplayLayer->getEnemyShips().size() <= 0) {
        if (!_spawned) {
            if (!isCleared()) {
                scheduleOnce([&](float) { _world->_mapLayer->drawPath(); }, 1.f, "draw_path");
                scheduleOnce([&](float) { spawnNextWave(); }, 5.f, "next_wave");
                _spawned = true;
            } else {
                _world->_hudLayer->notify('I', "All waves are cleared!");
                _world->endGame(true);
                unscheduleUpdate();
            }
        }
    }
}

void SpawnManager::spawnNextWave() {
    auto tier = getTier();
    ValueVector wave;

    if (tier == PREDEFINED) {
        wave = _patterns.at(PREDEFINED).asValueVector().at(_currentWave).asValueVector();
    } else if (tier == RANDOM) {
        auto tierVector = _patterns.at(RANDOM).asValueVector();
        auto rand = RandomHelper::random_int(0, (int) tierVector.size() - 1);
        wave = tierVector.at((unsigned long) rand).asValueVector();
    }

    _world->spawnWave(wave);

    _currentWave++;
    _spawned = false;
}

void SpawnManager::reset() {
    _currentWave = 0;
    _spawned = false;

    scheduleOnce([&](float delta) { scheduleUpdate(); }, START_DELAY, "start");
}

SpawnManager::Tier SpawnManager::getTier() {
    auto wave = getCurrentWave();

    if (wave >= 1 && wave <= 6)
        return PREDEFINED;
    else if (wave >= 7 && wave <= 100)
        return RANDOM;
    else return UNDEFINED;
}
