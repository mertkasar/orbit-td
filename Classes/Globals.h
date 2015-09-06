#ifndef GLOBALS_H
#define GLOBALS_H

#define SPRITE_ANGLE -90.f
#define STARTING_COIN 40
#define STARTING_LIFE 10

#define TURRET_CD 0.3f
#define TURRET_DMG 10.f
#define TURRET_COST 10

#define LASER_CD 0.f
#define LASER_DMG 0.3f
#define LASER_COST 30

#define R_LAUNCHER_CD 1.f
#define R_LAUNCHER_DMG 30.f
#define R_LAUNCHER_COST 50

#define MISSILE_MAX_VEL 160.f
#define MISSILE_EXPIRE_TIME 4.f //as seconds

#define ENEMY_MAX_VEL 80.f
#define ENEMY_HP 100.f
#define ENEMY_REWARD 5

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

enum CreepTypes {
    RAPTOR,
    SPEEDY,
    PANZER,
    PULSAR,
};

#endif //GLOBALS_H
