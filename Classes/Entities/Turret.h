#ifndef TURRET_H
#define TURRET_H

#include <Entities/Tower.h>

class Turret : public Tower {
public:
    virtual bool init();

    CREATE_FUNC(Turret);

private:
    virtual void shoot(float pDelta) override;
};

#endif //TURRET_H
