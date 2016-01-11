#ifndef PLANET_H
#define PLANET_H

#include <2d/CCNode.h>

class Planet : public cocos2d::Node {
public:
    virtual bool init();

    CREATE_FUNC(Planet);
};

#endif // PLANET_H
