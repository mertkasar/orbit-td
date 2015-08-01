#ifndef LASER_H
#define LASER_H

#include <Entities/Towers/Tower.h>

namespace cocos2d {
    class DrawNode;
}

class Laser : public Tower {
private:
    cocos2d::DrawNode *mTrace;

public:
    virtual bool init();

    CREATE_FUNC(Laser);

private:
    virtual void shoot(float pDelta) override;
};

#endif