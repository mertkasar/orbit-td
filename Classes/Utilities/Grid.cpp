#include <Utilities/Grid.h>

#include <iostream>

USING_NS_CC;

const Vec2 DIRS[4] = {Vec2(0, 1), Vec2(1, 0), Vec2(0, -1), Vec2(-1, 0)};

Grid::Grid() :
        mGrid() {
}

Grid::Grid(Vec2 size) {
    create(size);
}

bool Grid::create(Vec2 size, int value) {
    bool created = false;

    if (size.y > 0 && size.x > 0) {
        for (int i = 0; i < size.x; i++) {
            std::vector <int> row;

            for (int j = 0; j < size.y; j++)
                row.push_back(value);

            mGrid.push_back(row);
        }

        created = true;
    }

    return created;
}

void Grid::fill(int value) {
    Vec2 size = getSize();

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            mGrid[i][j] = value;
        }
    }
}

Grid::Locations Grid::getNeighbours(Vec2 node) const {
    Locations locs;

    if (isBounded(node)) {
        for (auto dir : DIRS) {
            Vec2 neighbour = node + dir;

            if (isBounded(neighbour))
                locs.push_back(neighbour);
        }
    }

    return locs;
}

Vec2 Grid::getSize() const {
    Vec2 size;

    size.x = mGrid.size();
    size.y = mGrid.at(0).size();

    return size;
}

bool Grid::isBounded(Vec2 node) const {
    Vec2 bounds = getSize();

    bool iBound = node.x >= 0 && node.x < bounds.x;
    bool jBound = node.y >= 0 && node.y < bounds.y;

    return iBound && jBound;
}

void Grid::print() {
    Vec2 size = getSize();

    std::cout << "Row:\t" << size.x << std::endl << "Column:\t" << size.y
    << std::endl;

    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            std::cout << mGrid[i][j] << ", ";
        }
        std::cout << std::endl;
    }
}