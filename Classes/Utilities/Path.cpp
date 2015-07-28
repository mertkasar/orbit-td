#include <Utilities/Path.h>

#include <2d/CCDrawNode.h>

USING_NS_CC;

Path::Path() : mWaypoints(), mCurrentNode(0) {
}

Path::~Path() {

}

void Path::addWaypoint(Vec2 pPosition, float pDensity) {
    mWaypoints.push_back(std::make_pair(pPosition, pDensity));
}

void Path::forward() {
    if (!eop())
        mCurrentNode++;
}

void Path::clear() {
    mWaypoints.clear();
    mCurrentNode = 0;
}
