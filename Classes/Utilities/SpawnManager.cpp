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
    auto tier = getTier();
    ValueVector wave;

    if (tier == PREDEFINED) {
        wave = _patterns.at(tier).asValueVector().at(_currentWave).asValueVector();
    } else {
        auto tierVector = _patterns.at(tier).asValueVector();
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

    if (wave >= 1 && wave <= 5)
        return PREDEFINED;
    else if (wave >= 6 && wave <= 33)
        return LOW;
    else if (wave >= 34 && wave <= 66)
        return MID;
    else if (wave >= 67 && wave <= 100)
        return HIGH;
    else return UNDEFINED;
}
