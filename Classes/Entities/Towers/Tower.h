#ifndef TOWER_H
#define TOWER_H

#include <2d/CCNode.h>

namespace cocos2d {
    class Sprite;

    class PhysicsBody;
}

class Creep;

class Tower : public cocos2d::Node {
protected:
    cocos2d::Sprite *mBase;
    cocos2d::Sprite *mGun;
    cocos2d::Sprite *mRange;
    cocos2d::PhysicsBody *mBody;

    Creep *mTarget;
    cocos2d::Vector<Creep *> mTargeList;

    float mCooldown;

    unsigned int mCost;

    bool mVerbose;

public:
    ~Tower();

    bool init(std::string pBaseTexturePath, std::string pGunTexturePath, float pRangeRadius, float pCooldown,
              unsigned int pCost);

    virtual void update(float pDelta);

    void addTarget(Creep *pTarget);

    void removeTarget(Creep *pTarget);

    unsigned int getTargetCount() const {
        return (unsigned int) mTargeList.size();
    }

    unsigned int getCost() const {
        return mCost;
    }

    bool isVerbose() const {
        return mVerbose;
    }

    void setVerbose(bool pVerbose);

protected:
    bool isTargetValid();

    void findTarget();

    virtual void adaptRotation();

    virtual void shoot(float pDelta) = 0;
};


#endif // TOWER_H