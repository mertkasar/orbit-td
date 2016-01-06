#ifndef GAMEPLAY_LAYER_H
#define GAMEPLAY_LAYER_H

#include <2d/CCLayer.h>
#include <Utilities/Pool.h>
#include <Globals.h>
#include <Entities/Missile.h>
#include <base/CCValue.h>

class World;

class Tower;

class Creep;

class Path;

class Explosion;

class Bullet;

namespace cocos2d {
    class ParticleBatchNode;
}

class GameplayLayer : public cocos2d::Layer {
private:
    GameplayLayer(World *world);

public:
    static GameplayLayer *create(World *world);

    virtual bool init();

    virtual void update(float delta);

    void addTower(ModelID type, cocos2d::Vec2 tile);

    void addEnemy(const cocos2d::ValueMap &model, int order, Path &path);

    void addMissile(cocos2d::Vec2 position, const cocos2d::Color3B &baseColor, float damage, Creep *target);

    void addBullet(cocos2d::Vec2 position, const cocos2d::Color3B &baseColor, float pDamage, Creep *target);

    void addExplosion(cocos2d::Vec2 position, float duration, float strength);

    Tower *getTower(cocos2d::Vec2 tile);

    void deleteTower(cocos2d::Vec2 tile);

    void pauseScene();

    void resumeScene();

    void updateEnemyPaths(const TraverseData &traversed, cocos2d::Vec2 goal);

    bool isEnemyPathsClear(const TraverseData &traversed, cocos2d::Vec2 node);

    bool isPaused() const {
        return _paused;
    }

    cocos2d::Vector<Creep *> &getCreepList() {
        return _creeps;
    }

private:
    World *_world;

    bool _paused;

    Pool<Creep> _creepPool;
    Pool<Missile> _missilePool;
    Pool<Bullet> _bulletPool;
    Pool<Explosion> _explosionPool;

    cocos2d::Vector<Creep *> _creeps;
    cocos2d::Vector<Missile *> _missiles;

    std::map<cocos2d::Vec2, Tower *> _towerMap;

    cocos2d::ParticleBatchNode *_particleBatch;
};

#endif //GAMEPLAY_LAYER_H
