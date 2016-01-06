#ifndef MISSILE_H
#define MISSILE_H

#include <2d/CCNode.h>

namespace cocos2d {
    class Sprite;

    class PhysicsBody;

    class ParticleSystemQuad;
}

class Creep;

class Missile : public cocos2d::Node {
private:
    cocos2d::Sprite *mSprite;
    cocos2d::PhysicsBody *mBody;
    cocos2d::ParticleSystemQuad *mFireEmitter;

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

    void restart(cocos2d::Vec2 pPosition, const cocos2d::Color3B &pBaseColor, float pDamage, Creep *pTarget);

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

    cocos2d::ParticleSystemQuad *getEmitter() { return mFireEmitter; }

private:
    void die(float pDelta);
};


#endif // MISSILE_H