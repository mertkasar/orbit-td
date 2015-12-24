#ifndef BULLET_H
#define BULLET_H

#include <2d/CCNode.h>

namespace cocos2d {
    class Sprite;
}

class Creep;

class Bullet : public cocos2d::Node {
private:
    cocos2d::Sprite *mSprite;

    Creep *mTarget;
    cocos2d::Vec2 mTargetPosition;

    float mDamage;

public:
    Bullet();

    virtual ~Bullet();

    virtual bool init();

    virtual void update(float pDelta);

    CREATE_FUNC(Bullet);

    void ignite(cocos2d::Vec2 pPosition, const cocos2d::Color3B &pBaseColor, float pDamage, Creep *pTarget);

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
};

#endif //BULLET_H
