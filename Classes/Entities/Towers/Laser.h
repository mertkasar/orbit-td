#ifndef LASER_H
#define LASER_H

#include <Entities/Towers/Tower.h>

class Beam;

class Laser : public Tower {
public:
    virtual bool init();

    virtual void update(float delta);

    virtual void upgrade(cocos2d::Color3B &color);

    CREATE_FUNC(Laser);

private:
    virtual void shoot(float pDelta) override;

private:
    Beam *_beam;
};

#endif