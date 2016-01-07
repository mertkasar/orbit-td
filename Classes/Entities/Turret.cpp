#include "Turret.h"

#include <2d/CCSprite.h>
#include <2d/CCActionInterval.h>
#include <physics/CCPhysicsBody.h>

#include "EnemyShip.h"
#include <sstream>
#include <audio/include/SimpleAudioEngine.h>

USING_NS_CC;

#define COST_RATIO 0.1f
#define RANGE_RATIO 0.15f
#define DAMAGE_RATIO 0.1f
#define CD_RATIO -0.2f

Turret::~Turret() {
    CCLOG("Turret deleted");
}

bool Turret::init(const cocos2d::ValueMap &model) {
    if (!Node::init())
        return false;

    //_model = model;
    _cost = model.at("base_cost").asFloat();;
    _range = model.at("base_range").asFloat();
    _damage = model.at("base_damage").asFloat();
    _cooldown = model.at("base_cd").asFloat();

    _level = 0;
    _nextShooting = 0.f;
    _verbose = true;

    _gunSprite = Sprite::createWithSpriteFrameName(model.at("gun_sprite_name").asString());
    _gunSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

    _muzzlePoint = Node::create();
    auto size = _gunSprite->getContentSize();
    _muzzlePoint->setPosition(size.width, size.height / 2.f);

    _baseSprite = Sprite::createWithSpriteFrameName("base_0.png");
    _baseSprite->setScale(0.5f);

    _rangeSprite = Sprite::createWithSpriteFrameName("range.png");
    _rangeSprite->setVisible(false);
    _rangeSprite->setColor(Color::GREEN);
    _rangeSprite->setScale(2 * _range / _rangeSprite->getContentSize().width);
    _rangeSprite->runAction(RepeatForever::create(RotateBy::create(2.f, 30.f)));

    _body = createBody(_range);
    setPhysicsBody(_body);

    _target = nullptr;

    addChild(_rangeSprite);
    addChild(_baseSprite);

    _gunSprite->addChild(_muzzlePoint);
    addChild(_gunSprite);

    scheduleUpdate();

    return true;
}

void Turret::addTarget(EnemyShip *target) {
    _targetList.pushBack(target);
}

void Turret::removeTarget(EnemyShip *target) {
    auto found = _targetList.find(target);

    if (found != _targetList.end())
        _targetList.erase(found);
}

void Turret::update(float delta) {
    if (isTargetValid()) {
        adaptRotation();

        _nextShooting = _nextShooting - delta;

        if (_nextShooting <= 0.f) {
            shoot(delta);
            _nextShooting = _cooldown;
        }
    } else {
        if (_target != nullptr) {
            _targetList.eraseObject(_target);
            _target = nullptr;
            _nextShooting = 0.f;
        }

        findTarget();
    }
}

void Turret::upgrade(cocos2d::Color3B &color) {
    _level++;

    _cost = (unsigned int) (_cost + _cost * COST_RATIO);
    _range = _range + _range * RANGE_RATIO;
    _damage = _damage + _damage * DAMAGE_RATIO;
    _cooldown = _cooldown + _cooldown * CD_RATIO;

    _rangeSprite->setScale(_range / 150.f); // TODO: Hard-coded value!
    _rangeSprite->setColor(color);

    std::stringstream ss;
    ss << "base_" << _level << ".png";

    _baseSprite->initWithSpriteFrameName(ss.str());

    _body->removeFromWorld();
    _body = createBody(_range);
    setPhysicsBody(_body);

    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/upgrade.wav");
}

bool Turret::isTargetValid() {
    if (_target != nullptr) {
        auto found = _targetList.find(_target);

        return found != _targetList.end() && !_target->isDead();
    }

    return false;
}

PhysicsBody *Turret::createBody(float rangeRadius) {
    auto body = PhysicsBody::createCircle(rangeRadius);

    body->setCategoryBitmask(TOWER_RANGE_MASK);
    body->setContactTestBitmask(ENEMY_MASK);
    body->setCollisionBitmask(NULL_MASK);

    return body;
}

void Turret::findTarget() {
    if (!_targetList.empty())
        _target = _targetList.at(0);
}

void Turret::adaptRotation() {
    Vec2 diff = _target->getPosition() - getPosition();
    diff.normalize();

    auto angle = CC_RADIANS_TO_DEGREES(diff.getAngle());

    _gunSprite->setRotation(-1 * angle);
}

void Turret::setVerbose(bool verbose) {
    if (verbose) {
        _rangeSprite->setVisible(true);
        auto currentScale = _rangeSprite->getScale();
        _rangeSprite->setScale(0.f);
        _rangeSprite->runAction(ScaleTo::create(0.15f, currentScale));
    } else {
        _rangeSprite->setVisible(false);
    }


    _verbose = verbose;
}

const Color3B &Turret::getBaseColor() {
    return _rangeSprite->getColor();
}


