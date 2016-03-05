#ifndef GAMEPLAY_LAYER_H
#define GAMEPLAY_LAYER_H

#include "../Globals.h"
#include "../Utilities/Pool.h"
#include "../Entities/Missile.h"

#include <2d/CCLayer.h>
#include <base/CCValue.h>

class World;

class Turret;

class EnemyShip;

class Path;

class Explosion;

class Bullet;

namespace cocos2d {
    class ParticleBatchNode;
}

class GameplayLayer : public cocos2d::Layer {
private:
    GameplayLayer(World *world);

    ~GameplayLayer();

public:
    static GameplayLayer *create(World *world);

    virtual bool init();

    virtual void update(float delta);

    void close(float delta = 0);

    void reset();

    void addTower(ModelID type, cocos2d::Vec2 tile);

    void addEnemyShip(const cocos2d::ValueMap &model, unsigned int order, Path &path, unsigned int waveCount);

    void addMissile(cocos2d::Vec2 position, const cocos2d::Color3B &baseColor, float damage, EnemyShip *target);

    void addBullet(cocos2d::Vec2 position, const cocos2d::Color3B &baseColor, float damage, EnemyShip *target);

    void addExplosion(cocos2d::Vec2 position, float duration, float strength);

    Turret *getTower(cocos2d::Vec2 tile);

    void deleteTower(cocos2d::Vec2 tile);

    void pauseScene();

    void resumeScene();

    void updateEnemyPaths(const TraverseData &traversed, cocos2d::Vec2 goal);

    bool isEnemyPathsClear(const TraverseData &traversed, cocos2d::Vec2 node);

    bool isPaused() const {
        return _paused;
    }

    cocos2d::Vector<EnemyShip *> &getEnemyShips() {
        return _ships;
    }

    void balanceTotalCoin(int balance) {
        _totalCoin = _totalCoin + balance;
    }

    unsigned int getTotalCoin() const {
        return _totalCoin;
    }

    void balanceRemainingLife(int balance) {
        _life = _life + balance;
    }

    unsigned int getRemainingLife() const {
        return _life;
    }

private:
    World *_world;

    unsigned int _totalCoin;
    unsigned int _life;

    bool _paused;

    Pool<EnemyShip> _shipPool;
    Pool<Missile> _missilePool;
    Pool<Bullet> _bulletPool;
    Pool<Explosion> _explosionPool;

    cocos2d::Vector<EnemyShip *> _ships;
    cocos2d::Vector<Missile *> _missiles;
    cocos2d::Vector<Bullet *> _bullets;

    std::map<cocos2d::Vec2, Turret *> _towerMap;

    cocos2d::ParticleBatchNode *_particleBatch;
};

#endif //GAMEPLAY_LAYER_H
