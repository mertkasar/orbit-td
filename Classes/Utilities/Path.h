#ifndef PATH_H
#define PATH_H

#include <vector>

#include <math/Vec2.h>

#include <Utilities/Algorithm.h>

struct WayPoint {
    cocos2d::Vec2 tile;
    cocos2d::Vec2 location;
    float reachRadius;
};

class Path {
private:
    std::vector<WayPoint> mWaypoints;
    std::vector<cocos2d::Vec2> mBuffer;

    unsigned int mIndex;

    WayPoint mNextWaypoint;
    WayPoint mCurrentWaypoint;

public:
    Path();

    ~Path();

    void addWaypoint(WayPoint pWaypoint);

    bool isReached(const TraverseData &pTraversed, cocos2d::Vec2 pDestination);

    void constructPath(const TraverseData &pTraversed, cocos2d::Vec2 pStart, cocos2d::Vec2 pGoal);

    void forward();

    void clone(const Path &pPath);

    void clear();

    const WayPoint &getNextWaypoint() const {
        return mNextWaypoint;
    }

    const WayPoint &getCurrentWaypoint() const {
        return mCurrentWaypoint;
    }

    const std::vector<WayPoint> &getWayPoints() const {
        return mWaypoints;
    }

    const unsigned int getIndex() const {
        return mIndex;
    };

    bool eop() const {
        return mIndex >= mWaypoints.size() - 1;
    }
};

#endif //PATH_H
