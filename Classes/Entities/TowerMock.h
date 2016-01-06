#ifndef TOWER_MOCK_H
#define TOWER_MOCK_H

#include <2d/CCNode.h>
#include <Globals.h>

class TowerMock : public cocos2d::Node  {
public:
    virtual bool init();

    CREATE_FUNC(TowerMock);

    void setSkin(ModelID pType);
};

#endif //ORBIT_TD_MOCK_H
