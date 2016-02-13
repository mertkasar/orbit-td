#ifndef COST_INDICATOR_H
#define COST_INDICATOR_H

#include <ui/UILayout.h>

namespace cocos2d{
    class Sprite;

    class Label;
}

class CostIndicator : public cocos2d::ui::Layout {
private:
    CostIndicator();

    ~CostIndicator();

public:
    static CostIndicator *create();

    virtual bool init();

    void update(int cost);

private:
    cocos2d::Label *_costLabel;
};

#endif //COST_INDICATOR_H
