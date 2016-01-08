#ifndef WORLD_H
#define WORLD_H

#include "../Globals.h"

#include <2d/CCLayer.h>
#include <base/CCValue.h>

namespace cocos2d {
    class DrawNode;

    class Label;

    class PhysicsWorld;
};

namespace CocosDenshion {
    class SimpleAudioEngine;
}

class MapLayer;

class GameplayLayer;

class HUDLayer;

class WheelMenu;

class World : public cocos2d::Layer {
public:
    World();

    ~World();

    static cocos2d::Scene *createScene();

    virtual bool init();

    virtual void update(float delta);

    CREATE_FUNC(World);

    bool placeTower(ModelID type, cocos2d::Vec2 tile);

    void destroyTower(cocos2d::Vec2 tile);

    void upgradeTower(cocos2d::Vec2 tile);

    bool spawnNextWave();

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

    cocos2d::PhysicsWorld *getPhysicsWorld() const {
        return __physicsWorld;
    }

    const cocos2d::ValueMap &getModel(unsigned int id) const {
        return _models.at(id);
    }

    void setPhysicsWorld(cocos2d::PhysicsWorld *_physicsWorld) {
        __physicsWorld = _physicsWorld;
    }

    bool isCleared() {
        return _cleared;
    }

private:
    void loadResources();

    void buildScene();

    void connectListeners();

    void loadModel(std::string path);

private:
    cocos2d::Size _visibleSize;
    cocos2d::Vec2 _origin;
    cocos2d::Vec2 _canvasCenter;

    cocos2d::PhysicsWorld *__physicsWorld;

    std::unordered_map<unsigned int, cocos2d::ValueMap> _models;

    cocos2d::ValueVector _waves;
    unsigned int _currentWave;
    bool _cleared;

    WheelMenu *_wheelMenu;

    unsigned int _totalCoin;

    unsigned int _life;

public:
    CocosDenshion::SimpleAudioEngine *_audioEngine;

    cocos2d::LayerColor *_backgroundLayer;
    MapLayer *_mapLayer;
    GameplayLayer *_gameplayLayer;
    HUDLayer *_hudLayer;

    std::vector<cocos2d::Color3B> _colors;
};

#endif // WORLD_H
