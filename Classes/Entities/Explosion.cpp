#include <2d/CCSprite.h>

#include <Entities/Explosion.h>
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

    mSprite = Sprite::create("textures/explosion.png");
    this->addChild(mSprite);
    this->setScale(1.5f);

    return true;
}

void Explosion::restart(cocos2d::Vec2 pPosition) {
    this->setPosition(pPosition);

    //Create an explosion animation among 3 different styles
    int row = random(0, 2);
    auto animation = createAnimation("textures/explosion.png", row, 128, 0.05f);

    //Find the current frame the created animation and set mSprite's spriteFrame
    int index = animation->getCurrentFrameIndex();
    auto currentFrame = animation->getAnimation()->getFrames().at(index)->getSpriteFrame();
    mSprite->setSpriteFrame(currentFrame);

    auto endCallback = CallFunc::create(CC_CALLBACK_0(Explosion::end, this));
    mSprite->runAction(Sequence::create(animation, endCallback, NULL));
}

void Explosion::end() {
    removeFromParent();
}

cocos2d::Animate *Explosion::createAnimation(std::string pPath, int pRow, int pDimension, float pInterval) {
    //Adding animation frames
    Vector<SpriteFrame *> explosionFrames;
    auto N = 12;

    explosionFrames.reserve(N);
    for (int i = 0; i < N; i++)
        explosionFrames.pushBack(SpriteFrame::create(pPath, Rect(i * pDimension, pRow * pDimension,
                                                                 pDimension, pDimension)));

    auto animation = Animation::createWithSpriteFrames(explosionFrames, pInterval);

    return Animate::create(animation);
}

void Explosion::pause() {
    Node::pause();

    mSprite->pause();
}

void Explosion::resume() {
    Node::resume();

    mSprite->resume();
}
