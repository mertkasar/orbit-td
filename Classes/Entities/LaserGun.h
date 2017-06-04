#ifndef LASER_GUN_H
#define LASER_GUN_H

#include "Turret.h"

class LaserBeam;

class LaserGun : public Turret {
public:
    static LaserGun *create(const cocos2d::ValueMap &model);

    virtual bool init(const cocos2d::ValueMap &model);

    virtual void update(float delta);

    virtual void upgrade();

private:
    virtual void shoot(float pDelta);

private:
    LaserBeam *_beam;
};

#endif // LASER_GUN_H
