#ifndef GLOBALS_H
#define GLOBALS_H

#include <base/ccTypes.h>

// TODO: THIS IS A BAD IDEA! FIX IT!
#include <base/CCValue.h>
extern std::unordered_map<unsigned int, cocos2d::ValueMap> _models;

//Collision bitmasks
#define NULL_MASK 0x00
#define TOWER_RANGE_MASK 0x01
#define ENEMY_MASK 0x02
#define MISSILE_MASK 0x03

enum ModelID {
    MACHINE_GUN,
    LASER_GUN,
    MISSILE_LAUNCHER,
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
    const cocos2d::Color3B BG = {45, 45, 45};
}

typedef std::map<cocos2d::Vec2, cocos2d::Vec2> TraverseData;

#endif //GLOBALS_H
