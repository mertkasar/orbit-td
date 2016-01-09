#ifndef ENEMY_SHIP_H
#define ENEMY_SHIP_H

#include "../Utilities/Path.h"

#include <2d/CCNode.h>
#include <base/CCValue.h>

namespace cocos2d {
    class Sprite;

    class PhysicsBody;
}

class EnemyShip : public cocos2d::Node {
public:
    EnemyShip();

    virtual ~EnemyShip();

    virtual bool init();

    virtual void update(float delta);

    CREATE_FUNC(EnemyShip);

    void restart(const cocos2d::ValueMap &model, cocos2d::Vec2 position, const Path &path);

    void deal(float damage);

    Path &getPath() {
        return _path;
    }

    float getHP() const {
        return _maxHP;
    }

    unsigned int getReward() const {
        return _reward;
    }

    bool isKilled() const {
        return _killed;
    }

    bool isReachedEnd() const {
        return _reachedEnd;
    }

    bool isDead() const {
        return _killed || _reachedEnd;
    }

private:
    void updateHPBar();

private:
    cocos2d::Sprite *_sprite;
    cocos2d::PhysicsBody *_body;
    cocos2d::Node *_HPBar;

    Path _path;

    float _maxHP;
    float _currentHP;
    unsigned int _reward;

    bool _killed;
    bool _reachedEnd;
};


#endif // ENEMY_SHIP_H