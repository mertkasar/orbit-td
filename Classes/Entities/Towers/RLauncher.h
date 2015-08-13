#ifndef R_LAUNCHER_H
#define R_LAUNCHER_H

#include <Entities/Towers/Tower.h>

class RLauncher : public Tower {
public:
    virtual bool init();

    CREATE_FUNC(RLauncher);

private:
    virtual void shoot(float pDelta) override;
};

#endif //R_LAUNCHER_H
