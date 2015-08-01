#ifndef ENEMY_H
#define ENEMY_H

#include <2d/CCNode.h>
#include <Utilities/Path.h>

namespace cocos2d {
    class Sprite;

    class PhysicsBody;
}

class Enemy : public cocos2d::Node {
private:
    cocos2d::Sprite *mSprite;
    cocos2d::PhysicsBody *mBody;

    Path mPath;

    float mHitPoints;

public:
    Enemy();

    virtual ~Enemy();

    virtual bool init();

    virtual void update(float pDelta);

    CREATE_FUNC(Enemy);

    Path &getPath() {
        return mPath;
    }

    void deal(float pDamage) {
        mHitPoints = mHitPoints - pDamage;
    }

    float getHP() const {
        return mHitPoints;
    }
};


#endif // ENEMY_H