#ifndef LASER_GUN_H
#define LASER_GUN_H

#include <Entities/Turret.h>

class LaserBeam;

class LaserGun : public Turret {
public:
    virtual bool init();

    virtual void update(float delta);

    virtual void upgrade(cocos2d::Color3B &color);

    CREATE_FUNC(LaserGun);

private:
    virtual void shoot(float pDelta);

private:
    LaserBeam *_beam;
};

#endif // LASER_GUN_H