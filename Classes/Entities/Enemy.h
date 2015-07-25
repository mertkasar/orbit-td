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
    cocos2d::Vec2 mTarget;

public:
    Enemy();

    virtual ~Enemy();

    virtual bool init();

    virtual void update(float pDelta);

    CREATE_FUNC(Enemy);

    const cocos2d::Vec2 &getTarget() const {
        return mTarget;
    }

    void setTarget(const cocos2d::Vec2 &pTarget) {
        mTarget = pTarget;
    }

    void constructPath(const Path &pPath);
};


#endif // ENEMY_H