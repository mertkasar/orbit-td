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

    Enemy *mTarget;
    cocos2d::Vector<Enemy *> mRange;

public:
    Tower();

    virtual ~Tower();

    virtual bool init();

    virtual void update(float pDelta);

    CREATE_FUNC(Tower);

    void addTarget(Enemy *pTarget);

    void removeTarget(Enemy *pTarget);

    unsigned int getTargetCount() const {
        return (unsigned int) mRange.size();
    }

    bool isTargetValid();

private:
    void findTarget();

    void adaptRotation();

    virtual void shoot(float pDelta);
};


#endif // TOWER_H