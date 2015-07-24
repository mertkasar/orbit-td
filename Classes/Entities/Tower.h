#ifndef TOWER_H
#define TOWER_H

#include <2d/CCNode.h>

namespace cocos2d {
    class Sprite;

    class PhysicsBody;
}

class Enemy;

class Tower : public cocos2d::Node {
private:
    cocos2d::Sprite *mBase;
    cocos2d::Sprite *mGun;
    cocos2d::PhysicsBody *mBody;

public:
    Tower();

    virtual ~Tower();

    virtual bool init();

    CREATE_FUNC(Tower);
};


#endif // TOWER_H