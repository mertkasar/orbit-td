#ifndef PATH_H
#define PATH_H

#include <math/Vec2.h>

#include <vector>

namespace cocos2d {
    class DrawNode;
}

class Path {
private:
    std::vector<cocos2d::Vec2> mWayPoints;

    unsigned int mCurrentNode;
    float mDensity;

public:
    Path();

    ~Path();

    void addNode(cocos2d::Vec2 pPosition);

    void forward();

    cocos2d::Vec2 getCurrentNode() const {
        return mWayPoints.at(mCurrentNode);
    }

    float getDensity() const {
        return mDensity;
    }

    const std::vector<cocos2d::Vec2> &getWayPointList() const {
        return mWayPoints;
    }

    bool eop() const {
        return mCurrentNode >= mWayPoints.size() - 1;
    }

    void setDensity(float pDensity) {
        mDensity = pDensity;
    }
};

#endif //PATH_H
