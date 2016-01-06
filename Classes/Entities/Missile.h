#ifndef MISSILE_H
#define MISSILE_H

#include <2d/CCNode.h>

namespace cocos2d {
    class Sprite;

    class PhysicsBody;

    class ParticleSystemQuad;
}

class Creep;

class Missile : public cocos2d::Node {
public:
    Missile();

    virtual ~Missile();

    virtual bool init();

    virtual void update(float delta);

    CREATE_FUNC(Missile);

    void restart(cocos2d::Vec2 position, const cocos2d::Color3B &baseColor, float damage, Creep *target);

    Creep *getTarget() const {
        return _target;
    }

    void setTarget(Creep *target) {
        _target = target;
    }

    float getDamage() const {
        return _damage;
    }

    void setDamage(float damage) {
        _damage = damage;
    }

    bool isDead() const {
        return _dead;
    }

    cocos2d::ParticleSystemQuad *getEmitter() { return _fireEmitter; }

private:
    void die(float pDelta);

private:
    cocos2d::Sprite *_sprite;
    cocos2d::PhysicsBody *_body;
    cocos2d::ParticleSystemQuad *_fireEmitter;

    Creep *_target;
    cocos2d::Vec2 _targetPosition;

    float _damage;

    bool _dead;
};


#endif // MISSILE_H