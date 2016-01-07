#ifndef TURRET_H
#define TURRET_H

#include <2d/CCNode.h>
#include <Globals.h>

namespace cocos2d {
    class Sprite;

    class PhysicsBody;

    class Color;
}

class EnemyShip;


class Turret : public cocos2d::Node {
public:
    ~Turret();

    bool init(const cocos2d::ValueMap &model);

    virtual void update(float delta);

    void addTarget(EnemyShip *target);

    void removeTarget(EnemyShip *target);

    virtual void upgrade(cocos2d::Color3B &color);

    unsigned int getTargetCount() const {
        return (unsigned int) _targetList.size();
    }

    unsigned int getCost() const {
        return _cost;
    }

    unsigned int getLevel() const {
        return _level;
    }

    const cocos2d::Color3B &getBaseColor();

    bool isVerbose() const {
        return _verbose;
    }

    bool isUpgradeable() const {
        return _level < 4;
    }

    void setVerbose(bool verbose);

protected:
    cocos2d::PhysicsBody *createBody(float rangeRadius);

    bool isTargetValid();

    void findTarget();

    virtual void adaptRotation();

    virtual void shoot(float delta) = 0;

protected:
    cocos2d::Sprite *_baseSprite;
    cocos2d::Sprite *_gunSprite;
    cocos2d::Sprite *_rangeSprite;
    cocos2d::PhysicsBody *_body;
    cocos2d::Node *_muzzlePoint;

    EnemyShip *_target;
    cocos2d::Vector<EnemyShip *> _targetList;

    //cocos2d::ValueMap &_model;

    unsigned int _cost;
    float _range;
    float _damage;
    float _cooldown;

    unsigned int _level;
    float _nextShooting;
    bool _verbose;
};


#endif // TURRET_H