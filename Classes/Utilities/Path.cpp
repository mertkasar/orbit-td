#include <Utilities/Path.h>

#include <Globals.h>

USING_NS_CC;

Path::Path() : mWaypoints(), mIndex(0), mNextWaypoint(), mCurrentWaypoint() {
}

Path::~Path() {

}

void Path::addWaypoint(WayPoint pWaypoint) {
    mWaypoints.push_back(pWaypoint);
}

bool Path::isReached(const TraverseData &pTraversed, cocos2d::Vec2 pDestination) {
    return (bool) pTraversed.count(pDestination);
}

void Path::constructPath(const TraverseData &pTraversed, cocos2d::Vec2 pStart, Vec2 pGoal) {
    this->clear();

    auto current = pStart;
    while (current != pGoal) {
        this->addWaypoint(WayPoint{current, algorithm::toCircularGrid(current), DEFAULT_WAYPOINT_DENSITY});
        current = pTraversed.find(current)->second;
    }

    this->addWaypoint(WayPoint{pGoal, algorithm::toCircularGrid(pGoal), DEFAULT_WAYPOINT_DENSITY});

    mNextWaypoint = mWaypoints.at(mIndex);
    mCurrentWaypoint = mWaypoints.at(mIndex);
}

void Path::forward() {
    if (!eop()) {
        mIndex++;

        mCurrentWaypoint = mNextWaypoint;
        mNextWaypoint = mWaypoints.at(mIndex);
    }
}

void Path::clone(const Path &pPath) {
    this->clear();

    for (auto waypoint : pPath.getWayPoints())
        this->addWaypoint(waypoint);

    mNextWaypoint = pPath.getNextWaypoint();
    mCurrentWaypoint = pPath.getCurrentWaypoint();
    mIndex = pPath.getIndex();
}

void Path::clear() {
    mWaypoints.clear();

    mNextWaypoint = WayPoint();
    mCurrentWaypoint = WayPoint();
    mIndex = 0;
}
