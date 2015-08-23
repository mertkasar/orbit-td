#ifndef R_LAUNCHER_H
#define R_LAUNCHER_H

#include <Entities/Towers/Tower.h>
#include <Utilities/Pool.h>

class Missile;

class RLauncher : public Tower {
private:
    static Pool<Missile> mMissilePool;
public:
    virtual bool init();

    CREATE_FUNC(RLauncher);

    static unsigned int getMissilePoolSize() {
        return mMissilePool.getSize();
    }

private:
    virtual void shoot(float pDelta) override;
};

#endif //R_LAUNCHER_H
