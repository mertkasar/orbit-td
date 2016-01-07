#ifndef MISSILE_LAUNCHER_H
#define MISSILE_LAUNCHER_H

#include <Entities/Turret.h>

class MissileLauncher : public Turret {
public:
    virtual bool init();

    CREATE_FUNC(MissileLauncher);

private:
    virtual void shoot(float delta);
};

#endif //MISSILE_LAUNCHER_H
