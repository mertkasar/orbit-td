#ifndef TOWER_MOCK_H
#define TOWER_MOCK_H

#include "../Globals.h"

#include <2d/CCNode.h>
#include <base/CCValue.h>

class TowerMock : public cocos2d::Node  {
public:
    virtual bool init();

    CREATE_FUNC(TowerMock);

    void update(const cocos2d::ValueMap &model);
};

#endif //TOWER_MOCK_H
