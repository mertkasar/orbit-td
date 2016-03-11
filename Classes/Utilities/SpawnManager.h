#ifndef SPAWN_MANAGER_H
#define SPAWN_MANAGER_H

#include <base/CCValue.h>
#include <2d/CCNode.h>

#define MAX_WAVE 100;

class World;

class SpawnManager : public cocos2d::Node {
private:
    SpawnManager(World *world);

    ~SpawnManager();

public:
    static SpawnManager *create(World *world);

    virtual bool init();

    virtual void update(float delta);

    void spawnNextWave(float delta);

    void reset();

    const unsigned int getCurrentWave() const {
        return _currentWave + 1;
    };

    const unsigned int getMaxWave() const {
        return (const unsigned int) _waves.size();
    };

    bool isCleared() const {
        return _currentWave >= _waves.size();
    }

private:
    World *_world;

    cocos2d::ValueVector _waves;

    unsigned int _currentWave;
    bool _spawned;
};

#endif
