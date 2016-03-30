#ifndef SPAWN_MANAGER_H
#define SPAWN_MANAGER_H

#include <base/CCValue.h>
#include <2d/CCNode.h>

#define MAX_WAVE 100;

class World;

class SpawnManager : public cocos2d::Node {
private:
    enum Tier {
        PREDEFINED,
        RANDOM,
        UNDEFINED
    };
private:
    SpawnManager(World *world);

    ~SpawnManager();

public:
    static SpawnManager *create(World *world);

    virtual bool init();

    virtual void update(float delta);

    void spawnNextWave();

    void reset();

    const unsigned int getCurrentWave() const {
        return _currentWave + 1;
    };

    const unsigned int getMaxWave() const {
        return MAX_WAVE;
    };

    bool isCleared() const {
        return _currentWave >= MAX_WAVE;
    }

private:
    Tier getTier();

private:
    World *_world;

    cocos2d::ValueVector _patterns;

    unsigned int _currentWave;
    bool _spawned;
};

#endif
