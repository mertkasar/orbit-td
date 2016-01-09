#include "MapLayer.h"

#include "World.h"

#include <2d/CCDrawNode.h>
#include <2d/CCActionInterval.h>
#include <2d/CCSprite.h>

#include <queue>

#define NODE_TOUCH_SIZE 50.f

USING_NS_CC;

MapLayer::MapLayer(World *world) {
    _world = world;
}

MapLayer *MapLayer::create(World *world) {
    MapLayer *layer = new(std::nothrow) MapLayer(world);

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

    //Draw Planet
    auto planet = DrawNode::create();
    planet->drawSolidCircle(Vec2(-480.f, 360.f), 600.f, 0.f, 50, Color4F::BLUE);
    addChild(planet);

    // Prepare sample grid
    _grid.create(Vec2(5, 10));
    for (int i = 0; i < _grid.getSize().x; i++)
        _grid.setNode(Vec2(i, 0), 2);
    _grid.setNode(Vec2(2, 0), 0);

    Vec2 size = _grid.getSize();
    for (int j = 0; j < size.y; j++) {
        Sprite *orbit = Sprite::createWithSpriteFrameName("orbit.png");
        orbit->setColor(Color::GREY);
        orbit->setPosition(algorithm::toCircularGrid(Vec2(2, j)));
        addChild(orbit);
    }

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            if (_grid.getNode(Vec2(i, j)) == 0) {
                Vec2 position = algorithm::toCircularGrid(Vec2(i, j));

                Sprite *shadow = Sprite::createWithSpriteFrameName("touch_shadow.png");
                shadow->setColor(Color::BG);
                shadow->setPosition(position);
                addChild(shadow);

                Sprite *touchArea = Sprite::createWithSpriteFrameName("touch.png");
                touchArea->setColor(Color::GREY);
                touchArea->setPosition(position);

                _slotMap.insert(std::make_pair(Vec2(i, j), touchArea));
                addChild(touchArea);
            }
        }
    }

    _start = Vec2(2, 9);
    _goal = Vec2(2, 0);

    _pathCanvas = DrawNode::create();

    auto traversed = traverseAgainst(_start, 0);
    if (_path.isReached(traversed, _start)) {
        _path.construct(traversed, _start, _goal);
        drawPath();
    }

    addChild(_pathCanvas);

    return true;
}

void MapLayer::activateSlot(cocos2d::Vec2 tile) {
    auto found = _slotMap.find(tile);

    assert(found != _slotMap.end());

    Sprite *slot = found->second;
    slot->setColor(Color::GREEN);
    slot->runAction(RepeatForever::create(RotateBy::create(2.f, 30.f)));
}

void MapLayer::deactivateSlot(cocos2d::Vec2 tile) {
    auto found = _slotMap.find(tile);

    assert(found != _slotMap.end());

    Sprite *slot = found->second;
    slot->setColor(Color::GREY);
    slot->stopAllActions();
    slot->setRotation(0.f);
}

void MapLayer::setSlotColor(cocos2d::Vec2 tile, cocos2d::Color3B color) {
    auto found = _slotMap.find(tile);

    assert(found != _slotMap.end());

    Sprite *slot = found->second;
    slot->setColor(color);
}

TraverseData MapLayer::traverseAgainst(Vec2 node, unsigned int value) {
    // Prepare a fake grid out of the current one
    Grid grid = _grid;
    grid.setNode(node, value);

    // Traverse fake grid with reversed Breadth-First Search
    std::queue<Vec2> frontier;
    TraverseData traversed;

    frontier.push(_goal);
    traversed[_goal] = _goal;

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
    _pathCanvas->clear();
    auto waypoints = _path.getWayPoints();

    for (unsigned int i = 0; i < waypoints.size(); i++) {
        auto waypoint = waypoints.at(i);

        _pathCanvas->drawSolidCircle(waypoint._location, 6.f, 0.f, 50, Color4F::RED);
        _pathCanvas->drawCircle(waypoint._location, waypoint._reachRadius, 0.f, 50, false, Color4F::RED);

        if (i != 0) {
            auto previousWaypoint = waypoints.at(i - 1);
            _pathCanvas->drawLine(waypoint._location, previousWaypoint._location, Color4F::RED);
        }
    }
}

Vec2 MapLayer::getTouchedSlot(Vec2 location) {
    Vec2 touched = Vec2(-1, -1);
    Vec2 size = _grid.getSize();
    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            Vec2 current = Vec2(i, j);
            Vec2 position = algorithm::toCircularGrid(current);
            Rect boundingBox = Rect(position.x - NODE_TOUCH_SIZE / 2.f,
                                    position.y - NODE_TOUCH_SIZE / 2.f,
                                    NODE_TOUCH_SIZE, NODE_TOUCH_SIZE);

            if (boundingBox.containsPoint(location)) {
                touched = current;
                break;
            }
        }
    }

    return touched;
}

bool MapLayer::isPathClear(const TraverseData &traversed) {
    return _path.isReached(traversed, _start);
}

void MapLayer::updateMap(const TraverseData &traversed, Vec2 tile, int value) {
    _grid.setNode(tile, value);
    _path.construct(traversed, _start, _goal);
    drawPath();
}
