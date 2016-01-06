#include <Entities/Creep.h>

#include <2d/CCSprite.h>
#include <2d/CCAction.h>
#include <physics/CCPhysicsBody.h>
#include <Utilities/SteeringDirector.h>
#include <base/CCValue.h>

USING_NS_CC;

#define HBAR_WIDTH 2.f
#define HBAR_HEIGHT 8.f

Creep::Creep() {
    CCLOG("Creep created");
}

Creep::~Creep() {
    CCLOG("Creep deleted");
}

bool Creep::init() {
    if (!Node::init())
        return false;

    _killed = false;
    _reachedEnd = false;

    _sprite = Sprite::create();
    _sprite->setContentSize(Size(100, 90));

    _body = PhysicsBody::createCircle(_sprite->getContentSize().width / 2.f);
    _body->setCategoryBitmask(ENEMY_MASK);
    _body->setContactTestBitmask(TOWER_RANGE_MASK);
    _body->setCollisionBitmask(NULL_MASK);
    this->setPhysicsBody(_body);

    _HPBar = Node::create();
    _HPBar->setPosition(Vec2(80.f, -40.f));

    auto hpBarB = Sprite::createWithSpriteFrameName("hp_bar.png");
    hpBarB->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    hpBarB->setName("bg");
    hpBarB->setColor(Color::RED);
    hpBarB->setScale(HBAR_WIDTH, HBAR_HEIGHT);
    _HPBar->addChild(hpBarB);

    auto hpBarF = Sprite::createWithSpriteFrameName("hp_bar.png");
    hpBarF->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    hpBarF->setName("fg");
    hpBarF->setColor(Color::GREEN);
    hpBarF->setScale(HBAR_WIDTH, HBAR_HEIGHT);
    _HPBar->addChild(hpBarF);

    this->addChild(_sprite);
    this->addChild(_HPBar);

    return true;
}

void Creep::update(float delta) {
    WayPoint target = _path.getNextWaypoint();
    float reachRadius = target._reachRadius + _sprite->getContentSize().width / 2.f;
    if (this->getPosition().distance(target._location) <= reachRadius) {
        if (_path.eop())
            _reachedEnd = true;
        else {
            _path.forward();
            target = _path.getNextWaypoint();
        }
    }

    SteeringDirector::getInstance()->seek(this, target._location);

    // Adapt rotation
    auto angle = CC_RADIANS_TO_DEGREES(_body->getVelocity().getAngle());
    _sprite->setRotation(-angle);

    if (_currentHP <= 0.f)
        _killed = true;
}

void Creep::restart(const cocos2d::ValueMap &model, cocos2d::Vec2 position, const Path &path) {
    _killed = false;
    _reachedEnd = false;

    _sprite->initWithSpriteFrameName(model.at("sprite").asString());
    _maxHP = model.at("max_hp").asFloat();
    _reward = model.at("reward").asFloat();
    _body->setMass(model.at("mass").asFloat());
    _body->setVelocityLimit(model.at("max_velocity").asFloat());

    _currentHP = _maxHP;

    this->setPosition(position);
    this->setScale(0.5f);
    _body->setVelocity(Vec2::ZERO);

    _path.clone(path);

    updateHPBar();

    this->scheduleUpdate();
}

void Creep::deal(float damage) {
    _currentHP = _currentHP - damage;

    if (_currentHP < 0.f)
        _currentHP = 0.f;

    updateHPBar();
}

void Creep::updateHPBar() {
    auto fg = _HPBar->getChildByName("fg");
    auto ratio = _currentHP / _maxHP;
    fg->setScaleY(HBAR_HEIGHT * ratio);
}
