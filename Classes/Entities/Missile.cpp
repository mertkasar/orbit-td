#include <Entities/Missile.h>

#include <2d/CCSprite.h>
#include <2d/CCAction.h>
#include <physics/CCPhysicsBody.h>

#include <Utilities/SteeringDirector.h>
#include <Entities/Creep.h>
#include <2d/CCParticleSystem.h>
#include <2d/CCParticleExamples.h>
#include <Utilities/Shake.h>

USING_NS_CC;

#define MISSILE_MAX_VEL 160.f
#define MISSILE_EXPIRE_TIME 4.f //as seconds

Missile::Missile() {
    CCLOG("Missile created");
}

Missile::~Missile() {
    CCLOG("Missile deleted");
}

bool Missile::init() {
    if (!Node::init())
        return false;

    _sprite = Sprite::createWithSpriteFrameName("missile.png");

    _body = PhysicsBody::createCircle(_sprite->getContentSize().width / 2.f);
    _body->setCategoryBitmask(MISSILE_MASK);
    _body->setContactTestBitmask(NULL_MASK);
    _body->setCollisionBitmask(NULL_MASK);
    _body->setMass(5.5f);
    _body->setVelocityLimit(MISSILE_MAX_VEL);
    this->setPhysicsBody(_body);

    _fireEmitter = ParticleSystemQuad::create("missile_fire.plist");
    _fireEmitter->setPosition(this->getPosition());

    this->addChild(_sprite);

    return true;
}

void Missile::update(float delta) {
    _fireEmitter->setPosition(this->getPosition());

    if (_target != nullptr) {
        if (!_target->isDead())
            _targetPosition = _target->getPosition();
        else
            _target = nullptr;
    }

    float reachRadius = 10 + _sprite->getContentSize().width / 2.f;
    if (this->getPosition().distance(_targetPosition) <= reachRadius) {
        die(delta);

        if (_target != nullptr) {
            _target->deal(_damage);
            _target->runAction(Shake::actionWithDuration(0.4f, 1.3f));
        }
    }

    SteeringDirector::getInstance()->seek(this, _targetPosition);

    // Adapt rotation
    auto angle = CC_RADIANS_TO_DEGREES(_body->getVelocity().getAngle());
    _sprite->setRotation(-angle);

    if (isDead()) {
        _fireEmitter->stopSystem();
        removeFromParent();
    }
}

void Missile::restart(cocos2d::Vec2 position, const cocos2d::Color3B &baseColor, float damage, Creep *target) {
    _dead = false;

    this->setPosition(position);
    setDamage(damage);
    setTarget(target);
    _body->setVelocity(Vec2::ZERO);

    this->setScale(0.5f);

    _fireEmitter->setStartColor(Color4F(baseColor));

    //TODO: This doesn't work as I expected, I need a resume operation not a reset!
    _fireEmitter->resetSystem();

    this->scheduleUpdate();
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(Missile::die), MISSILE_EXPIRE_TIME);
}

void Missile::die(float pDelta) {
    if (!_dead)
        _dead = true;
}
