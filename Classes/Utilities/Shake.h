#ifndef __SHAKE_H__
#define __SHAKE_H__

#include <2d/CCActionInterval.h>

namespace cocos2d{
    class Node;
}

class Shake : public cocos2d::ActionInterval {
    // Code by Francois Guibert
    // Contact: www.frozax.com - http://twitter.com/frozax - www.facebook.com/frozax
public:
    Shake();

    // Create the action with a time and a strength (same in x and y)
    static Shake *actionWithDuration(float d, float strength);

    // Create the action with a time and strengths (different in x and y)
    static Shake *actionWithDuration(float d, float strength_x, float strength_y);

    bool initWithDuration(float d, float strength_x, float strength_y);

    virtual void startWithTarget(cocos2d::Node *pTarget);

    virtual void update(float time);

    virtual void stop(void);

    virtual Shake *clone();


protected:
    // Initial position of the shaked node
    float _initial_x, _initial_y;
    // Strength of the action
    float _strength_x, _strength_y;
};

#endif //__SHAKE_H__
