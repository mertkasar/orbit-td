#ifndef BEAM_H
#define BEAM_H

#include <2d/CCNode.h>

class Beam : public cocos2d::Node {
private:
    cocos2d::Node *mStart;
    cocos2d::Node *mMid;
    cocos2d::Node *mEnd;

public:
    Beam();

    virtual ~Beam();

    virtual bool init();

    void update(cocos2d::Vec2 pEnd);

    CREATE_FUNC(Beam);

    void setColor(const cocos2d::Color3B &pBaseColor);
};

#endif //BEAM_H
