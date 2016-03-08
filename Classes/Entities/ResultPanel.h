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

    ~ResultPanel();

public:
    static ResultPanel *create(World *world, bool win, int point, int energy, int shield);

    bool init(bool win, int point, int energy, int shield);

    virtual void update(float delta);

    cocos2d::FiniteTimeAction *show();

    cocos2d::FiniteTimeAction *hide();

private:
    void replayButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

    void menuButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

private:
    World *_world;

    cocos2d::ui::Text *_titleText;
    cocos2d::ui::Text *_scoreText;

    int _score;
    int _scoreCounter;
    int _incrementVal;
};

#endif //RESULT_PANEL_H
