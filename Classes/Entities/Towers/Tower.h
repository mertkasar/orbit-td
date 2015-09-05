#ifndef TOWER_H
#define TOWER_H

#include <2d/CCNode.h>

namespace cocos2d {
    class Sprite;

    class PhysicsBody;
}

class Enemy;

class Tower : public cocos2d::Node {
protected:
    cocos2d::Sprite *mBase;
    cocos2d::Sprite *mGun;
    cocos2d::PhysicsBody *mBody;

    Enemy *mTarget;
    cocos2d::Vector<Enemy *> mRange;

    float mCooldown;

    unsigned int mCost;

public:
    bool init(std::string pBaseTexturePath, std::string pGunTexturePath, float pRangeRadius, float pCooldown,
              unsigned int pCost);

    void addTarget(Enemy *pTarget);

    void removeTarget(Enemy *pTarget);

    unsigned int getTargetCount() const {
        return (unsigned int) mRange.size();
    }

    unsigned int getCost() const {
        return mCost;
    }

    virtual void update(float pDelta);

protected:
    void findTarget();

    virtual void adaptRotation();

    virtual void shoot(float pDelta) = 0;
};


#endif // TOWER_H