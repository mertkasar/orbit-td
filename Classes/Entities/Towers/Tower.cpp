#include <Entities/Towers/Tower.h>

#include <2d/CCSprite.h>
#include <2d/CCActionInterval.h>
#include <physics/CCPhysicsBody.h>

#include <Entities/Creep.h>
#include <sstream>
#include <audio/include/SimpleAudioEngine.h>

USING_NS_CC;

#define COST_RATIO 0.1f
#define RANGE_RATIO 0.15f
#define DAMAGE_RATIO 0.1f
#define CD_RATIO -0.2f

bool Tower::init(const TowerModel &model) {
    if (!Node::init())
        return false;

    _model = model;
    _cost = model.baseCost;
    _range = model.baseRange;
    _damage = model.baseDamage;
    _cooldown = model.baseCD;

    _level = 0;
    _nextShooting = 0.f;
    _verbose = true;

    _gunSprite = Sprite::createWithSpriteFrameName(model.gunSpritePath);
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
    this->setPhysicsBody(_body);

    _target = nullptr;

    this->addChild(_rangeSprite);
    this->addChild(_baseSprite);

    _gunSprite->addChild(_muzzlePoint);
    this->addChild(_gunSprite);

    this->scheduleUpdate();

    return true;
}

void Tower::addTarget(Creep *target) {
    _targetList.pushBack(target);
}

void Tower::removeTarget(Creep *target) {
    auto found = _targetList.find(target);

    if (found != _targetList.end())
        _targetList.erase(found);
}

void Tower::update(float delta) {
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

void Tower::upgrade(cocos2d::Color3B &color) {
    _level++;

    _cost = (unsigned int) (_cost + _cost * COST_RATIO);
    _range = _range + _range * RANGE_RATIO;
    _damage = _damage + _damage * DAMAGE_RATIO;
    _cooldown = _cooldown + _cooldown * CD_RATIO;

    _rangeSprite->setScale(_range / _model.baseRange);
    _rangeSprite->setColor(color);

    std::stringstream ss;
    ss << "base_" << _level << ".png";

    _baseSprite->initWithSpriteFrameName(ss.str());

    _body->removeFromWorld();
    _body = createBody(_range);
    this->setPhysicsBody(_body);

    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/upgrade.wav");
}

bool Tower::isTargetValid() {
    if (_target != nullptr) {
        auto found = _targetList.find(_target);

        return found != _targetList.end() && !_target->isDead();
    }

    return false;
}

PhysicsBody *Tower::createBody(float rangeRadius) {
    auto body = PhysicsBody::createCircle(rangeRadius);

    body->setCategoryBitmask(TOWER_RANGE_MASK);
    body->setContactTestBitmask(ENEMY_MASK);
    body->setCollisionBitmask(NULL_MASK);

    return body;
}

void Tower::findTarget() {
    if (!_targetList.empty())
        _target = _targetList.at(0);
}

void Tower::adaptRotation() {
    Vec2 diff = _target->getPosition() - this->getPosition();
    diff.normalize();

    auto angle = CC_RADIANS_TO_DEGREES(diff.getAngle());

    _gunSprite->setRotation(-1 * angle);
}

void Tower::setVerbose(bool verbose) {
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

Tower::~Tower() {
    CCLOG("Tower deleted");
}

const Color3B &Tower::getBaseColor() {
    return _rangeSprite->getColor();
}
