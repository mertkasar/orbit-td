#ifndef ENEMY_H
#define ENEMY_H

#include <2d/CCNode.h>
#include <Utilities/Path.h>

namespace cocos2d {
    class Sprite;

    class PhysicsBody;

    class DrawNode;
}

class Enemy : public cocos2d::Node {
private:
    cocos2d::Sprite *mSprite;
    cocos2d::PhysicsBody *mBody;

    Path mPath;

    float mMaxHP;
    float mCurrentHP;
    cocos2d::DrawNode *mHPBar;

public:
    Enemy();

    virtual ~Enemy();

    virtual bool init();

    virtual void update(float pDelta);

    CREATE_FUNC(Enemy);

    void deal(float pDamage);

    Path &getPath() {
        return mPath;
    }

    float getHP() const {
        return mMaxHP;
    }

private:
    void updateHPBar();
};


#endif // ENEMY_H