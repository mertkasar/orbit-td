#include <Utilities/Path.h>

#include <2d/CCDrawNode.h>

USING_NS_CC;

Path::Path() : mWaypoints(), mIndex(0) {
}

Path::~Path() {

}

void Path::addWaypoint(WayPoint pWaypoint) {
    mWaypoints.push_back(pWaypoint);
}

void Path::forward() {
    if (!eop())
        mIndex++;
}

void Path::clear() {
    mWaypoints.clear();
    mIndex = 0;
}
