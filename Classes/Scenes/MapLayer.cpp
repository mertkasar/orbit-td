#include <Scenes/MapLayer.h>

#include <2d/CCDrawNode.h>
#include <2d/CCActionInterval.h>
#include <2d/CCSprite.h>

#include <Scenes/World.h>
#include <2d/CCSpriteFrameCache.h>
#include <queue>

USING_NS_CC;

#define NODE_TOUCH_SIZE 50.f

MapLayer::MapLayer(World *pWorld) {
    mWorld = pWorld;
}

MapLayer *MapLayer::create(World *pWorld) {
    MapLayer *layer = new(std::nothrow) MapLayer(pWorld);

    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    } else {
        delete layer;
        layer = NULL;
        return NULL;
    }
}

bool MapLayer::init() {
    if (!Layer::init())
        return false;

    this->setName("map_layer");

    auto spriteCache = SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile("textures/map_layer.plist");

    //Draw Planet
    auto planet = DrawNode::create();
    planet->drawSolidCircle(Vec2(-480.f, 360.f), 600.f, 0.f, 50, Color4F::BLUE);
    this->addChild(planet);

    // Prepare sample grid
    mGrid.create(Vec2(5, 10));
    for (int i = 0; i < mGrid.getSize().x; i++)
        mGrid.setNode(Vec2(i, 0), 2);
    mGrid.setNode(Vec2(2, 0), 0);

    Vec2 size = mGrid.getSize();
    for (int j = 0; j < size.y; j++) {
        Sprite *orbit = Sprite::createWithSpriteFrameName("orbit.png");
        orbit->setColor(Color::GREY);
        orbit->setPosition(algorithm::toCircularGrid(Vec2(2, j)));
        this->addChild(orbit);
    }

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            if (mGrid.getNode(Vec2(i, j)) == 0) {
                Vec2 position = algorithm::toCircularGrid(Vec2(i, j));

                Sprite *shadow = Sprite::createWithSpriteFrameName("touch_shadow.png");
                shadow->setColor(Color::BG);
                shadow->setPosition(position);
                this->addChild(shadow);

                Sprite *touchArea = Sprite::createWithSpriteFrameName("touch.png");
                touchArea->setColor(Color::GREY);
                touchArea->setPosition(position);

                mSlotMap.insert(std::make_pair(Vec2(i, j), touchArea));
                this->addChild(touchArea);
            }
        }
    }

    mStart = Vec2(2, 9);
    mGoal = Vec2(2, 0);

    mPathCanvas = DrawNode::create();

    auto traversed = traverseAgainst(mStart, 0);
    if (mPath.isReached(traversed, mStart)) {
        mPath.construct(traversed, mStart, mGoal);
        drawPath();
    }

    this->addChild(mPathCanvas);

    return true;
}

void MapLayer::activateSlot(cocos2d::Vec2 pTile) {
    auto found = mSlotMap.find(pTile);

    assert(found != mSlotMap.end());

    Sprite *slot = found->second;
    slot->setColor(Color::GREEN);
    slot->runAction(RepeatForever::create(RotateBy::create(2.f, 30.f)));
}

void MapLayer::deactivateSlot(cocos2d::Vec2 pTile) {
    auto found = mSlotMap.find(pTile);

    assert(found != mSlotMap.end());

    Sprite *slot = found->second;
    slot->setColor(Color::GREY);
    slot->stopAllActions();
    slot->setRotation(0.f);
}

void MapLayer::setSlotColor(cocos2d::Vec2 pTile, cocos2d::Color3B pColor) {
    auto found = mSlotMap.find(pTile);

    assert(found != mSlotMap.end());

    Sprite *slot = found->second;
    slot->setColor(pColor);
}

TraverseData MapLayer::traverseAgainst(Vec2 pNode, unsigned int pValue) {
    // Prepare a fake grid out of the current one
    Grid grid = mGrid;
    grid.setNode(pNode, pValue);

    // Traverse fake grid with reversed Breadth-First Search
    std::queue<Vec2> frontier;
    TraverseData traversed;

    frontier.push(mGoal);
    traversed[mGoal] = mGoal;

    while (!frontier.empty()) {
        auto current = frontier.front();
        frontier.pop();

        auto neighbors = grid.getNeighbours(current);

        //Cross-movement improve on grid
        if ((int) (current.x + current.y) % 2 == 0)
            std::reverse(neighbors.begin(), neighbors.end());

        for (auto next : neighbors) {
            if (!traversed.count(next) && grid.getNode(next) == 0) {
                frontier.push(next);
                traversed[next] = current;
            }
        }
    }

    return traversed;
}

void MapLayer::drawPath() {
    mPathCanvas->clear();
    auto waypoints = mPath.getWayPoints();

    for (unsigned int i = 0; i < waypoints.size(); i++) {
        auto waypoint = waypoints.at(i);

        mPathCanvas->drawSolidCircle(waypoint.location, 6.f, 0.f, 50, Color4F::RED);
        mPathCanvas->drawCircle(waypoint.location, waypoint.reachRadius, 0.f, 50, false, Color4F::RED);

        if (i != 0) {
            auto previousWaypoint = waypoints.at(i - 1);
            mPathCanvas->drawLine(waypoint.location, previousWaypoint.location, Color4F::RED);
        }
    }
}

Vec2 MapLayer::getTouchedSlot(Vec2 pLocation) {
    Vec2 touched = Vec2(-1, -1);
    Vec2 size = mGrid.getSize();
    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            Vec2 current = Vec2(i, j);
            Vec2 location = algorithm::toCircularGrid(current);
            Rect boundingBox = Rect(location.x - NODE_TOUCH_SIZE / 2.f,
                                    location.y - NODE_TOUCH_SIZE / 2.f,
                                    NODE_TOUCH_SIZE, NODE_TOUCH_SIZE);

            if (boundingBox.containsPoint(pLocation)) {
                touched = current;
                break;
            }
        }
    }

    return touched;
}

bool MapLayer::isPathClear(const TraverseData &pTraversed) {
    return mPath.isReached(pTraversed, mStart);
}

void MapLayer::updateMap(const TraverseData &pTraversed, Vec2 pTile, int pValue) {
    mGrid.setNode(pTile, pValue);
    mPath.construct(pTraversed, mStart, mGoal);
    drawPath();
}
