#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <2d/CCNode.h>

namespace cocos2d {
    class Sprite;

    class Animate;
}

class Creep;

class Explosion : public cocos2d::Node {
private:
    cocos2d::Sprite *mSprite;

public:
    Explosion();

    virtual ~Explosion();

    virtual bool init();

    virtual void pause();

    virtual void resume();

    CREATE_FUNC(Explosion);

    void restart(cocos2d::Vec2 pPosition);

private:
    void end();

    cocos2d::Animate *createAnimation(std::string pPath, int pRow, int pDimension, float pInterval);
};

#endif //EXPLOSION_H
