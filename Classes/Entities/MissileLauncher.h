#ifndef MISSILE_LAUNCHER_H
#define MISSILE_LAUNCHER_H

#include <Entities/Turret.h>

class MissileLauncher : public Turret {
public:
    static MissileLauncher *create(const cocos2d::ValueMap &model);

    virtual bool init(const cocos2d::ValueMap &model);

private:
    virtual void shoot(float delta);
};

#endif //MISSILE_LAUNCHER_H
