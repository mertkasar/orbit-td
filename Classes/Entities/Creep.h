#ifndef ENEMY_H
#define ENEMY_H

#include <2d/CCNode.h>
#include <Utilities/Path.h>
#include <base/CCValue.h>

namespace cocos2d {
    class Sprite;

    class PhysicsBody;
}

class Creep : public cocos2d::Node {
private:
    cocos2d::Sprite *mSprite;
    cocos2d::PhysicsBody *mBody;
    cocos2d::Node *mHPBar;

    Path mPath;

    float mMaxHP;
    float mCurrentHP;
    unsigned int mReward;

    bool mKilled;
    bool mReachedEnd;

public:
    Creep();

    virtual ~Creep();

    virtual bool init();

    virtual void update(float pDelta);

    CREATE_FUNC(Creep);

    void restart(const cocos2d::ValueMap &pModel, cocos2d::Vec2 pPosition, const Path &pPath);

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