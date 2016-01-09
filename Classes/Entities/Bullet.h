#ifndef BULLET_H
#define BULLET_H

#include <2d/CCNode.h>

namespace cocos2d {
    class Sprite;
}

class EnemyShip;

class Bullet : public cocos2d::Node {
public:
    Bullet();

    virtual ~Bullet();

    virtual bool init();

    virtual void update(float delta);

    CREATE_FUNC(Bullet);

    void restart(cocos2d::Vec2 position, const cocos2d::Color3B &baseColor, float damage, EnemyShip *target);

    EnemyShip *getTarget() const {
        return _target;
    }

    void setTarget(EnemyShip *target) {
        _target = target;
    }

    float getDamage() const {
        return _damage;
    }

    void setDamage(float damage) {
        _damage = damage;
    }

private:
    cocos2d::Sprite *_sprite;

    EnemyShip *_target;
    cocos2d::Vec2 _targetPosition;

    float _damage;

};

#endif //BULLET_H
