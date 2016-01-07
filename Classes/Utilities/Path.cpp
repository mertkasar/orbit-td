#include <Utilities/Path.h>

USING_NS_CC;

#define DEFAULT_WAYPOINT_DENSITY 15.f

Path::Path() : _waypoints(), _index(0), _nextWaypoint(), _currentWaypoint() {
}

Path::~Path() {

}

void Path::addWaypoint(WayPoint waypoint) {
    _waypoints.push_back(waypoint);
}

bool Path::isReached(const TraverseData &traversed, cocos2d::Vec2 destination) {
    return (bool) traversed.count(destination);
}

void Path::construct(const TraverseData &traversed, cocos2d::Vec2 start, Vec2 goal) {
    clear();

    auto current = start;
    while (current != goal) {
        addWaypoint(WayPoint{current, algorithm::toCircularGrid(current), DEFAULT_WAYPOINT_DENSITY});
        current = traversed.find(current)->second;
    }

    addWaypoint(WayPoint{goal, algorithm::toCircularGrid(goal), 0.f});

    _nextWaypoint = _waypoints.at(_index);
    _currentWaypoint = _waypoints.at(_index);
}

void Path::forward() {
    if (!eop()) {
        _index++;

        _currentWaypoint = _nextWaypoint;
        _nextWaypoint = _waypoints.at(_index);
    }
}

void Path::clone(const Path &path) {
    clear();

    for (auto waypoint : path.getWayPoints())
        addWaypoint(waypoint);

    _nextWaypoint = path.getNextWaypoint();
    _currentWaypoint = path.getCurrentWaypoint();
    _index = path.getIndex();
}

void Path::clear() {
    _waypoints.clear();

    _nextWaypoint = WayPoint();
    _currentWaypoint = WayPoint();
    _index = 0;
}
