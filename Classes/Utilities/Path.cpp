#include <Utilities/Path.h>

#include <2d/CCDrawNode.h>

USING_NS_CC;

Path::Path() : mWayPoints(), mCurrentNode(0), mDensity(10.f) {
}

Path::~Path() {

}

void Path::addNode(Vec2 pPosition) {
    mWayPoints.push_back(pPosition);
}

void Path::forward() {
    if (!eop())
        mCurrentNode++;
}
