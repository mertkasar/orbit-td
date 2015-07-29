#ifndef PATH_H
#define PATH_H

#include <math/Vec2.h>

#include <vector>

#include <Globals.h>

struct WayPoint {
    cocos2d::Vec2 tile;
    cocos2d::Vec2 location;
    float reachRadius;
};

class Path {
private:
    std::vector<WayPoint> mWaypoints;

    unsigned int mIndex;

public:
    Path();

    ~Path();

    void addWaypoint(WayPoint pWaypoint);

    void forward();

    void clear();

    WayPoint getWaypoint() const {
        return mWaypoints.at(mIndex);
    }

    const std::vector<WayPoint> &getWayPoints() const {
        return mWaypoints;
    }

    bool eop() const {
        return mIndex >= mWaypoints.size() - 1;
    }
};

#endif //PATH_H
