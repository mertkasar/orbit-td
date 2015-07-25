#ifndef STEERING_DIRECTOR_H
#define STEERING_DIRECTOR_H

#include <math/Vec2.h>

namespace cocos2d {
    class Node;
}

class SteeringDirector {
private:
    SteeringDirector();

public:
    static SteeringDirector *getInstance();

    void seek(cocos2d::Node *pSeeker, cocos2d::Vec2 pSeeking);
};

#endif //STEERING_DIRECTOR_H
