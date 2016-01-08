#ifndef PATH_H
#define PATH_H

#include "Algorithm.h"

#include <math/Vec2.h>

#include <vector>

struct WayPoint {
    cocos2d::Vec2 _tile;
    cocos2d::Vec2 _location;
    float _reachRadius;
};

class Path {
public:
    Path();

    ~Path();

    void addWaypoint(WayPoint waypoint);

    bool isReached(const TraverseData &traversed, cocos2d::Vec2 destination);

    void construct(const TraverseData &traversed, cocos2d::Vec2 start, cocos2d::Vec2 goal);

    void forward();

    void clone(const Path &path);

    void clear();

    const WayPoint &getNextWaypoint() const {
        return _nextWaypoint;
    }

    const WayPoint &getCurrentWaypoint() const {
        return _currentWaypoint;
    }

    const std::vector<WayPoint> &getWayPoints() const {
        return _waypoints;
    }

    const unsigned int getIndex() const {
        return _index;
    };

    bool eop() const {
        return _index >= _waypoints.size() - 1;
    }

private:
    std::vector<WayPoint> _waypoints;
    std::vector<cocos2d::Vec2> _buffer;

    unsigned int _index;

    WayPoint _nextWaypoint;
    WayPoint _currentWaypoint;
};

#endif //PATH_H
