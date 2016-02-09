#ifndef WORLD_H
#define WORLD_H

#include "../Globals.h"

#include <2d/CCLayer.h>
#include <base/CCValue.h>

namespace cocos2d {
    class PhysicsWorld;

    class Sprite;
};

namespace CocosDenshion {
    class SimpleAudioEngine;
}

class MapLayer;

class GameplayLayer;

class MainMenuLayer;

class HUDLayer;

class WheelMenu;

class Planet;

class World : public cocos2d::Layer {
public:
    enum State{
        MAIN_MENU,
        GAMEPLAY
    };

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

    State getState() const{
        return _currentState;
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

    void setState(State state);

    bool isCleared() {
        return _cleared;
    }

private:
    void loadResources();

    void connectListeners();

    void loadModel(std::string path);

private:
    cocos2d::PhysicsWorld *__physicsWorld;

    State _currentState;

    Planet *_planet;

    std::unordered_map<unsigned int, cocos2d::ValueMap> _models;

    cocos2d::ValueVector _waves;
    unsigned int _currentWave;
    bool _cleared;

    WheelMenu *_wheelMenu;

    unsigned int _totalCoin;

    unsigned int _life;

public:
    CocosDenshion::SimpleAudioEngine *_audioEngine;

    cocos2d::Size _visibleSize;
    cocos2d::Vec2 _origin;
    cocos2d::Vec2 _canvasCenter;

    cocos2d::Sprite *_backgroundSprite;
    MainMenuLayer *_mainMenuLayer;
    MapLayer *_mapLayer;
    GameplayLayer *_gameplayLayer;
    HUDLayer *_hudLayer;
};

#endif // WORLD_H
