#ifndef WORLD_H
#define WORLD_H

#include "../Globals.h"

#include <2d/CCLayer.h>
#include <base/CCValue.h>

namespace cocos2d {
    class PhysicsWorld;

    class Sprite;

    class UserDefault;
};

namespace CocosDenshion {
    class SimpleAudioEngine;
}

class MapLayer;

class GameplayLayer;

class MainMenuLayer;

class HUDLayer;

class Planet;

class SpawnManager;

class World : public cocos2d::Layer {
public:
    enum State {
        IDLE,
        MAIN_MENU,
        GAMEPLAY
    };

private:
    World();

    ~World();

public:
    static cocos2d::Scene *createScene();

    virtual bool init();

    void resetGame();

    CREATE_FUNC(World);

    bool placeTower(ModelID type, cocos2d::Vec2 tile);

    void destroyTower(cocos2d::Vec2 tile);

    void upgradeTower(cocos2d::Vec2 tile);

    void spawnWave(const cocos2d::ValueVector &waveData);

    void endGame(bool win);

    State getState() const {
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

private:
    void loadModels();

    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);


private:
    cocos2d::PhysicsWorld *__physicsWorld;

    State _currentState;

    Planet *_planet;

    std::unordered_map<unsigned int, cocos2d::ValueMap> _models;

public:
    CocosDenshion::SimpleAudioEngine *_audioEngine;
    cocos2d::UserDefault *_prefs;

    cocos2d::Size _visibleSize;
    cocos2d::Vec2 _origin;
    cocos2d::Vec2 _canvasCenter;

    cocos2d::Sprite *_backgroundSprite;
    MainMenuLayer *_mainMenuLayer;
    MapLayer *_mapLayer;
    GameplayLayer *_gameplayLayer;
    SpawnManager *_spawnManager;
    HUDLayer *_hudLayer;

    bool _debugMode;
};

#endif // WORLD_H
