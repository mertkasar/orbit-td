#ifndef MISSILE_H
#define MISSILE_H

#include <2d/CCNode.h>
#include <Utilities/Path.h>

namespace cocos2d {
    class Sprite;

    class PhysicsBody;
}

class Enemy;

class Missile : public cocos2d::Node {
private:
    cocos2d::Sprite *mSprite;
    cocos2d::PhysicsBody *mBody;

    Enemy *mTarget;
    cocos2d::Vec2 mTargetPosition;

    float mDamage;

    bool mDead;

public:
    Missile();

    virtual ~Missile();

    virtual bool init();

    virtual void update(float pDelta);

    CREATE_FUNC(Missile);

    Enemy *getTarget() const {
        return mTarget;
    }

    void setTarget(Enemy *pTarget) {
        mTarget = pTarget;
    }

    float getDamage() const {
        return mDamage;
    }

    void setDamage(float pDamage) {
        mDamage = pDamage;
    }

    bool isDead() const {
        return mDead;
    }

private:
    void die(float pDelta);
};


#endif // MISSILE_H