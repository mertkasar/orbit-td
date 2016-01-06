#ifndef BEAM_H
#define BEAM_H

#include <2d/CCNode.h>

class Beam : public cocos2d::Node {
public:
    Beam();

    virtual ~Beam();

    virtual bool init();

    void update(cocos2d::Vec2 end);

    CREATE_FUNC(Beam);

    void setColor(const cocos2d::Color3B &baseColor);

private:
    cocos2d::Node *_start;
    cocos2d::Node *_mid;
    cocos2d::Node *_end;
};

#endif //BEAM_H
