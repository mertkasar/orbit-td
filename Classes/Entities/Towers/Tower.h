#ifndef TOWER_H
#define TOWER_H

#include <2d/CCNode.h>

namespace cocos2d {
    class Sprite;

    class PhysicsBody;

    class Color;
}

class Creep;

class Tower : public cocos2d::Node {
protected:
    cocos2d::Sprite *mBase;
    cocos2d::Sprite *mGun;
    cocos2d::Sprite *mRange;
    cocos2d::PhysicsBody *mBody;

    Creep *mTarget;
    cocos2d::Vector<Creep *> mTargetList;

    unsigned int mLevel;

    float mNextShooting;
    float mCooldown;

    unsigned int mCost;

    bool mVerbose;

public:
    ~Tower();

    bool init(std::string pGunTexturePath, float pCooldown, unsigned int pCost);

    void build();

    virtual void update(float pDelta);

    void addTarget(Creep *pTarget);

    void removeTarget(Creep *pTarget);

    void upgrade();

    unsigned int getTargetCount() const {
        return (unsigned int) mTargetList.size();
    }

    unsigned int getCost() const {
        return mCost;
    }

    const cocos2d::Color3B &getBaseColor();

    bool isVerbose() const {
        return mVerbose;
    }

    bool isUpgradeable() const {
        return mLevel < 4;
    }

    void setVerbose(bool pVerbose);

protected:
    cocos2d::PhysicsBody *createBody(float pRangeRadius);

    void updateBody(float pScaleFactor);

    bool isTargetValid();

    void findTarget();

    virtual void adaptRotation();

    virtual void shoot(float pDelta) = 0;
};


#endif // TOWER_H