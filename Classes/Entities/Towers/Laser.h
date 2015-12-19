#ifndef LASER_H
#define LASER_H

#include <Entities/Towers/Tower.h>

class Beam;

class Laser : public Tower {
private:
    Beam *mBeam;

public:
    virtual bool init();

    virtual void update(float pDelta);

    virtual void upgrade(cocos2d::Color3B &pColor);

    CREATE_FUNC(Laser);

private:
    virtual void shoot(float pDelta) override;
};

#endif