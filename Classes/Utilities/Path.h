#ifndef PATH_H
#define PATH_H

#include <math/Vec2.h>

#include <vector>

#include <Globals.h>

namespace cocos2d {
    class DrawNode;
}

class Path {
public:
    typedef std::pair<cocos2d::Vec2, float> Waypoint;

private:
    std::vector<Waypoint> mWaypoints;

    unsigned int mCurrentNode;

public:
    Path();

    ~Path();

    void addWaypoint(cocos2d::Vec2 pPosition, float pDensity = DEFAULT_WAYPOINT_DENSITY);

    void forward();

    void clear();

    Waypoint getCurrentWaypoint() const {
        return mWaypoints.at(mCurrentNode);
    }

    const std::vector<Waypoint> &getWaypointList() const {
        return mWaypoints;
    }

    bool eop() const {
        return mCurrentNode >= mWaypoints.size() - 1;
    }
};

#endif //PATH_H
