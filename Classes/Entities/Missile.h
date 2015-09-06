#ifndef MISSILE_H
#define MISSILE_H

#include <2d/CCNode.h>
#include <Utilities/Path.h>

namespace cocos2d {
    class Sprite;

    class PhysicsBody;
}

class Creep;

class Missile : public cocos2d::Node {
private:
    cocos2d::Sprite *mSprite;
    cocos2d::PhysicsBody *mBody;

    Creep *mTarget;
    cocos2d::Vec2 mTargetPosition;

    float mDamage;

    bool mDead;

public:
    Missile();

    virtual ~Missile();

    virtual bool init();

    virtual void update(float pDelta);

    CREATE_FUNC(Missile);

    void ignite(cocos2d::Vec2 pPosition, float pDamage, Creep * pTarget);

    Creep *getTarget() const {
        return mTarget;
    }

    void setTarget(Creep *pTarget) {
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