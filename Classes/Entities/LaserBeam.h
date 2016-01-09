#ifndef LASER_BEAM_H
#define LASER_BEAM_H

#include <2d/CCNode.h>

class LaserBeam : public cocos2d::Node {
public:
    LaserBeam();

    virtual ~LaserBeam();

    virtual bool init();

    void update(cocos2d::Vec2 end);

    CREATE_FUNC(LaserBeam);

    void setColor(const cocos2d::Color3B &baseColor);

private:
    cocos2d::Node *_start;
    cocos2d::Node *_mid;
    cocos2d::Node *_end;
};

#endif //LASER_BEAM_H
