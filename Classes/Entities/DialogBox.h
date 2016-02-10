#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H

#include <ui/UILayout.h>

namespace cocos2d {
    namespace ui {
        class Text;
        class Button;
    }

    class FiniteTimeAction;
}

class World;

class DialogBox : public cocos2d::ui::Layout {
private:
    DialogBox(World *world);

    ~DialogBox();

public:
    static DialogBox *create(World *world);

    virtual bool init();

    void setCaption(std::string caption);

    void setAction(const ccWidgetTouchCallback& action);

    cocos2d::FiniteTimeAction *show();

    cocos2d::FiniteTimeAction *hide();

private:
    void noButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

private:
    World *_world;

    cocos2d::ui::Text *_text;
    cocos2d::ui::Button *_yesButton;
};

#endif //DIALOG_BOX_H
