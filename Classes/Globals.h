#ifndef GLOBALS_H
#define GLOBALS_H

#include <base/ccTypes.h>

//Collision bitmasks
#define NULL_MASK 0x00
#define TOWER_RANGE_MASK 0x01
#define ENEMY_MASK 0x02
#define MISSILE_MASK 0x03

enum TowerTypes {
    TURRET,
    LASER,
    R_LAUNCHER
};

struct TowerModel {
    TowerTypes type;
    std::string gunSpritePath;
    unsigned int baseCost;
    float baseRange;
    float baseDamage;
    float baseCD;
};

extern std::map<unsigned int, TowerModel> models;

enum CreepTypes {
    RAPTOR,
    SPEEDY,
    PANZER,
    PULSAR,
};

//Colors
namespace Color {
    const cocos2d::Color3B GREEN = {113, 201, 55};
    const cocos2d::Color3B YELLOW = {255, 204, 0};
    const cocos2d::Color3B BLUE = {54, 187, 245};
    const cocos2d::Color3B GREY = {108, 115, 131};
    const cocos2d::Color3B RED = {172, 57, 57};
    const cocos2d::Color3B BG = {42, 45, 51};
}


#endif //GLOBALS_H
