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
public:
    Creep();

    virtual ~Creep();

    virtual bool init();

    virtual void update(float delta);

    CREATE_FUNC(Creep);

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


#endif // ENEMY_H