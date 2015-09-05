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
    unsigned int mReward;

    bool mKilled;
    bool mReachedEnd;

public:
    Enemy();

    virtual ~Enemy();

    virtual bool init();

    virtual void update(float pDelta);

    CREATE_FUNC(Enemy);

    void ignite(cocos2d::Vec2 pPosition, const Path &pPath);

    void deal(float pDamage);

    Path &getPath() {
        return mPath;
    }

    float getHP() const {
        return mMaxHP;
    }

    unsigned int getReward() const {
        return mReward;
    }

    bool isKilled() const {
        return mKilled;
    }

    bool isReachedEnd() const {
        return mReachedEnd;
    }

    bool isDead() const {
        return mKilled || mReachedEnd;
    }

private:
    void updateHPBar();
};


#endif // ENEMY_H