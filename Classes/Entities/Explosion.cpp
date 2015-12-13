#include <2d/CCSprite.h>

#include <Entities/Explosion.h>
#include <2d/CCSpriteFrame.h>
#include <2d/CCAnimation.h>
#include <2d/CCActionInterval.h>

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

void Explosion::ignite(cocos2d::Vec2 pPosition) {
    this->setPosition(pPosition);

    int row = random(0, 2);
    auto animation = createAnimation("textures/explosion.png", row, 128, 0.05f);

    mSprite->runAction(RepeatForever::create(animation));
}

cocos2d::Animate *Explosion::createAnimation(std::string pPath, int pRow, int pDimension, float pInterval) {
    //Adding animation frames
    Vector<SpriteFrame *> explosionFrames;
    Size contentSize = mSprite->getContentSize();
    auto N = contentSize.width / pDimension;

    explosionFrames.reserve(N);
    for (int i = 0; i < N; i++)
        explosionFrames.pushBack(SpriteFrame::create(pPath, Rect(i * pDimension, pRow * pDimension,
                                                                 pDimension, pDimension)));

    auto animation = Animation::createWithSpriteFrames(explosionFrames, pInterval);

    return Animate::create(animation);
}
