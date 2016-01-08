#ifndef MACHINE_GUN_H
#define MACHINE_GUN_H

#include <Entities/Turret.h>

class MachineGun : public Turret {
public:
    static MachineGun *create(const cocos2d::ValueMap &model);

    virtual bool init(const cocos2d::ValueMap &model);

private:
    virtual void shoot(float delta);
};

#endif //MACHINE_GUN_H
