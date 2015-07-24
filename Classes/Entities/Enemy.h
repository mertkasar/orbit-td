#ifndef ENEMY_H
#define ENEMY_H

#include <2d/CCNode.h>

namespace cocos2d {
    class Sprite;

    class PhysicsBody;
}

class Enemy : public cocos2d::Node {
private:
    cocos2d::Sprite *mSprite;
    cocos2d::PhysicsBody *mBody;

public:
    Enemy();

    virtual ~Enemy();

    virtual bool init();

    CREATE_FUNC(Enemy);
};


#endif // ENEMY_H