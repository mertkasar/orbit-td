#ifndef BEAM_H
#define BEAM_H

#include <2d/CCNode.h>

namespace cocos2d {
    class Sprite;
}

class Creep;

class Beam : public cocos2d::Node {
private:
    cocos2d::Sprite *mStartB;
    cocos2d::Sprite *mStartF;
    cocos2d::Sprite *mMidB;
    cocos2d::Sprite *mMidF;

public:
    Beam();

    virtual ~Beam();

    virtual bool init();

    void update(cocos2d::Vec2 pEnd);

    CREATE_FUNC(Beam);

    void setColor(const cocos2d::Color3B &pBaseColor);
};

#endif //BEAM_H
