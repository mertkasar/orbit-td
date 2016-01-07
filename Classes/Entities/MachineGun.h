#ifndef MACHINE_GUN_H
#define MACHINE_GUN_H

#include <Entities/Turret.h>

class MachineGun : public Turret {
public:
    virtual bool init();

    CREATE_FUNC(MachineGun);

private:
    virtual void shoot(float delta);
};

#endif //MACHINE_GUN_H
