#include "Explosion.h"

#include <2d/CCSprite.h>
#include <2d/CCSpriteFrame.h>
#include <2d/CCAnimation.h>
#include <2d/CCActionInterval.h>
#include <2d/CCActionInstant.h>

USING_NS_CC;

Explosion::Explosion() {
    CCLOG("Explosion created");
}

Explosion::~Explosion() {
    CCLOG("Explosion deleted");
}

bool Explosion::init() {
    if (!Node::init())
        return false;

    setCascadeOpacityEnabled(true);

    _sprite = Sprite::create("textures/explosion.png");
    addChild(_sprite);
    setScale(1.5f);

    return true;
}

void Explosion::restart(cocos2d::Vec2 position) {
    setPosition(position);

    //Create an explosion animation among 3 different styles
    int row = random(0, 2);
    auto animation = createAnimation("textures/explosion.png", row, 128, 0.05f);

    //Find the current frame the created animation and set mSprite's spriteFrame
    int index = animation->getCurrentFrameIndex();
    auto currentFrame = animation->getAnimation()->getFrames().at(index)->getSpriteFrame();
    _sprite->setSpriteFrame(currentFrame);

    auto endCallback = CallFunc::create(CC_CALLBACK_0(Explosion::end, this));
    _sprite->runAction(Sequence::create(animation, endCallback, NULL));
}

void Explosion::end() {
    removeFromParent();
}

cocos2d::Animate *Explosion::createAnimation(std::string path, int row, int dimension, float interval) {
    //Adding animation frames
    Vector<SpriteFrame *> explosionFrames;
    auto N = 12;

    explosionFrames.reserve(N);
    for (int i = 0; i < N; i++)
        explosionFrames.pushBack(SpriteFrame::create(path, Rect(i * dimension, row * dimension,
                                                                dimension, dimension)));

    auto animation = Animation::createWithSpriteFrames(explosionFrames, interval);

    return Animate::create(animation);
}

void Explosion::pause() {
    Node::pause();

    _sprite->pause();
}

void Explosion::resume() {
    Node::resume();

    _sprite->resume();
}
