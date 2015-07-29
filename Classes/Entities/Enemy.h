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
    WayPoint mNextWaypoint;
    WayPoint mCurrentWaypoint;

public:
    Enemy();

    virtual ~Enemy();

    virtual bool init();

    virtual void update(float pDelta);

    CREATE_FUNC(Enemy);

    void constructPath(const Path &pPath);

    const WayPoint& getNextWaypoint() const {
        return mNextWaypoint;
    }

    const WayPoint& getCurrentWaypoint() const {
        return mCurrentWaypoint;
    }
};


#endif // ENEMY_H