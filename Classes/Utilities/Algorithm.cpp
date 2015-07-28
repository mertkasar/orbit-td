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

// Greedy Breadth-First Search
typedef std::pair<int, cocos2d::Vec2> PrioritizedTile;

class Comparator {
public:
    bool operator()(PrioritizedTile lhs, PrioritizedTile rhs) {
        return lhs.first > rhs.first;
    }
};

class Frontier {
private:
    std::priority_queue<PrioritizedTile, std::vector<PrioritizedTile>, Comparator> mQueue;

public:
    bool empty() {
        return mQueue.empty();
    }

    void put(int pPriority, Vec2 pFrontier) {
        mQueue.emplace(pPriority, pFrontier);
    }

    Vec2 get() {
        auto best = mQueue.top().second;
        mQueue.pop();

        return best;
    }
};

int heuristic(Vec2 pStart, Vec2 pEnd) {
    return (int) (std::abs(pStart.x - pEnd.x) + std::abs(pStart.y - pEnd.y));
}

TraverseData algorithm::traverse(const Grid &pGrid, const Vec2 pStart, const Vec2 pGoal) {
    Frontier frontier;
    TraverseData traversed;

    frontier.put(0, pStart);
    traversed.push_back(std::make_pair(pStart, pStart));

    while (!frontier.empty()) {
        Vec2 current = frontier.get();

        //Stop traversing when reached pGoal location
        if (current == pGoal) {
            break;
        }

        auto neighbours = pGrid.getNeighbours(current);

        //Cross-movement improve on grid
        if ((int) (current.x + current.y) % 2 == 0)
            std::reverse(neighbours.begin(), neighbours.end());

        for (auto next : neighbours) {
            if (pGrid.getNode(next) == 0) {

                bool isHave = false;
                for (auto pair : traversed) {
                    Vec2 loc = pair.second;

                    if (loc.x == next.x && loc.y == next.y)
                        isHave = true;
                }

                if (!isHave) {
                    auto priority = heuristic(pGoal, next);
                    frontier.put(priority, next);
                    traversed.push_back(std::make_pair(current, next));
                }
            }
        }
    }

    return traversed;
}

bool algorithm::isReached(const TraverseData &pTraversed, const cocos2d::Vec2 &pNode) {
    for (auto pair : pTraversed) {
        Vec2 loc = pair.second;

        if (loc.x == pNode.x && loc.y == pNode.y)
            return true;
    }

    return false;
}

std::vector<cocos2d::Vec2> algorithm::calculatePath(const TraverseData &pTraversed, const cocos2d::Vec2 pStart,
                                                    const cocos2d::Vec2 pGoal) {
    std::vector<cocos2d::Vec2> waypoints;
    Vec2 current = pGoal;

    while (current != pStart) {
        for (auto pair : pTraversed) {
            if (current == pair.second) {
                current = pair.first;

                waypoints.push_back(pair.second);
                break;
            }
        }
    }

    waypoints.push_back(pStart);

    std::reverse(waypoints.begin(), waypoints.end());

    return waypoints;
}

// Greedy Breadth-First Search

