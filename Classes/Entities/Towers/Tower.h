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
public:
    struct Model {
        unsigned int tag;
        std::string gunSpritePath;
        unsigned int baseCost;
        float baseRange;
        float baseDamage;
        float baseCD;
    };

protected:
    cocos2d::Sprite *mBaseSprite;
    cocos2d::Sprite *mGunSprite;
    cocos2d::Sprite *mRangeSprite;
    cocos2d::PhysicsBody *mBody;

    Creep *mTarget;
    cocos2d::Vector<Creep *> mTargetList;

    std::map<unsigned int, Model> mModels;
    Model mModel;

    unsigned int mLevel;
    unsigned int mCost;
    float mRange;
    float mDamage;
    float mCooldown;
    float mNextShooting;

    bool mVerbose;

public:
    ~Tower();

    bool init(Model pModel);

    void build();

    virtual void update(float pDelta);

    void addTarget(Creep *pTarget);

    void removeTarget(Creep *pTarget);

    void upgrade(cocos2d::Color3B &pColor);

    unsigned int getTargetCount() const {
        return (unsigned int) mTargetList.size();
    }

    unsigned int getCost() const {
        return mCost;
    }

    unsigned int getLevel() const {
        return mLevel;
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

    bool isTargetValid();

    void findTarget();

    virtual void adaptRotation();

    virtual void shoot(float pDelta) = 0;
};


#endif // TOWER_H