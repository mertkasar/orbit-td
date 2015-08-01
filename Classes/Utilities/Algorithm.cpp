#include <Utilities/Algorithm.h>

#include <queue>

#include <base/ccMacros.h>

#include <Globals.h>
#include <Utilities/Grid.h>

USING_NS_CC;

cocos2d::Vec2 algorithm::toCircularGrid(Vec2 pPriTile) {
    float angle = GRID_START_ANGLE - pPriTile.x * GRID_SHIFT_ANGLE;

    Vec2 position = Vec2((float) (GRID_RADIUS * cos(CC_DEGREES_TO_RADIANS(angle))),
                         (float) (GRID_RADIUS * sin(CC_DEGREES_TO_RADIANS(angle))));

    Vec2 shift = pPriTile.y * Vec2(100.f, 0);

    return position + shift;
}

//Breadth-First Search
TraverseData algorithm::traverse(const Grid &pGrid, const Vec2 pStart, const Vec2 pGoal) {
    std::queue<Vec2> frontier;
    TraverseData traversed;

    frontier.push(pGoal);
    traversed[pGoal] = pGoal;

    while (!frontier.empty()) {
        auto current = frontier.front();
        frontier.pop();

        auto neighbors = pGrid.getNeighbours(current);

        //Cross-movement improve on grid
        if ((int) (current.x + current.y) % 2 == 0)
            std::reverse(neighbors.begin(), neighbors.end());

        for (auto next : neighbors) {
            if (!traversed.count(next) && pGrid.getNode(next) == 0) {
                frontier.push(next);
                traversed[next] = current;
            }
        }
    }

    return traversed;
}

