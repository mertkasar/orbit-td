#include "MapLayer.h"

#include "World.h"

#include <2d/CCDrawNode.h>
#include <2d/CCActionInterval.h>
#include <2d/CCActionEase.h>
#include <2d/CCSprite.h>

#include <queue>
#include <2d/CCActionInstant.h>

#define NODE_TOUCH_SIZE 95.f

USING_NS_CC;

MapLayer::MapLayer(World *world) {
    _world = world;
    CCLOG("MapLayer created");
}

MapLayer::~MapLayer() {
    CCLOG("MapLayer deleted");
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

    setCascadeOpacityEnabled(true);

    // Prepare sample grid
    _grid.create(Vec2(5, 10));
    for (int i = 0; i < _grid.getSize().x; i++)
        _grid.setNode(Vec2(i, 0), 2);
    _grid.setNode(Vec2(2, 0), 0);

    float d = 2.5f;

    Vec2 size = _grid.getSize();
    for (int i = 0; i < size.y; i++) {
        Sprite *orbit = Sprite::createWithSpriteFrameName("orbit.png");
        orbit->setColor(Color::GREY);
        orbit->setPosition(algorithm::toCircularGrid(Vec2(2, i)));

        orbit->setOpacity((GLubyte) 0.f);
        orbit->runAction(Sequence::create(DelayTime::create(d + i * 0.1f), FadeIn::create(0.3f), NULL));

        addChild(orbit);
    }

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            if (_grid.getNode(Vec2(i, j)) == 0) {
                Vec2 position = algorithm::toCircularGrid(Vec2(i, j));
                auto scaleUpEffect = Sequence::create(DelayTime::create(d + 0.3f + j * 0.1f), ScaleTo::create(0.3f, 1.f), NULL);

                Sprite *shadow = Sprite::create("textures/touch_bg.png");
                shadow->setColor(Color::BG);
                shadow->setPosition(position);

                shadow->setScale(0.f);
                shadow->runAction(scaleUpEffect->clone());

                addChild(shadow);

                Sprite *touchArea = Sprite::create("textures/touch.png");
                touchArea->setColor(Color::GREY);
                touchArea->setPosition(position);

                _slotMap.insert(std::make_pair(Vec2(i, j), touchArea));

                touchArea->setScale(0.f);
                touchArea->runAction(Sequence::create(DelayTime::create(0.3f), scaleUpEffect->clone(), NULL));

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

void MapLayer::close(float delay) {
    runAction(Sequence::create(DelayTime::create(delay), FadeOut::create(0.3f), RemoveSelf::create(true), NULL));
}

void MapLayer::reset() {
    for (auto pair : _slotMap)
        deactivateSlot(pair.first);

    _grid.clear();
    _grid.create(Vec2(5, 10));
    for (int i = 0; i < _grid.getSize().x; i++)
        _grid.setNode(Vec2(i, 0), 2);
    _grid.setNode(Vec2(2, 0), 0);
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
   /* _pathCanvas->clear();
    auto waypoints = _path.getWayPoints();

    for (unsigned int i = 0; i < waypoints.size(); i++) {
        auto waypoint = waypoints.at(i);

        _pathCanvas->drawSolidCircle(waypoint._location, 6.f, 0.f, 50, Color4F::RED);
        _pathCanvas->drawCircle(waypoint._location, waypoint._reachRadius, 0.f, 50, false, Color4F::RED);

        if (i != 0) {
            auto previousWaypoint = waypoints.at(i - 1);
            _pathCanvas->drawLine(waypoint._location, previousWaypoint._location, Color4F::RED);
        }
    }*/
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
