#ifndef GLOBALS_H
#define GLOBALS_H

#define SPRITE_ANGLE -90.f

#define TOWER_CD 0.3f
#define TOWER_DMG 10.f

#define ENEMY_MAX_VEL 80.f
#define ENEMY_HP 100.f

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

//Type definitions
typedef std::map<cocos2d::Vec2, cocos2d::Vec2> TraverseData;

#endif //GLOBALS_H
