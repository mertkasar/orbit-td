#ifndef RESULT_PANEL_H
#define RESULT_PANEL_H

#include <ui/UILayout.h>

class World;

namespace cocos2d {
    namespace ui {
        class Text;
        class Button;
    }

    class FiniteTimeAction;
}

class ResultPanel : public cocos2d::ui::Layout {
private:
    ResultPanel(World *world);

public:
    static ResultPanel *create(World *world);

    virtual bool init();

    cocos2d::FiniteTimeAction *show();

    cocos2d::FiniteTimeAction *hide();

private:
    void replayButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

    void menuButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

private:
    World *_world;

    cocos2d::ui::Text *_header;
};

#endif //RESULT_PANEL_H
