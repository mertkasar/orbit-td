#ifndef GLOBALS_H
#define GLOBALS_H

#include <base/ccTypes.h>

#define SPRITE_ANGLE -90.f
#define STARTING_COIN 500
#define STARTING_LIFE 10

#define TURRET_COST 10
#define LASER_COST 30
#define R_LAUNCHER_COST 50

#define MISSILE_MAX_VEL 160.f
#define MISSILE_EXPIRE_TIME 4.f //as seconds

#define HBAR_WIDTH 20.f
#define HBAR_HEIGHT 80.f

#define GRID_RADIUS 600.f
#define GRID_START_ANGLE 20.f
#define GRID_SHIFT_ANGLE 10.f
#define DEFAULT_WAYPOINT_DENSITY 15.f
#define NODE_TOUCH_SIZE 50.f

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
}


#endif //GLOBALS_H
