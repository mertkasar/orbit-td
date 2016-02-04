#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H

#include <ui/UILayout.h>

namespace cocos2d {
    namespace ui {
        class Text;
        class Button;
    }
}

class World;

class DialogBox : public cocos2d::ui::Layout {
public:
    static DialogBox *create(World *world);

    bool init(World *world);

    void setCaption(std::string caption);

    void setAction(const ccWidgetTouchCallback& action);

private:
    void noButtonCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type);

private:
    World *_world;

    cocos2d::ui::Text *_text;
    cocos2d::ui::Button *_yesButton;
};

#endif //DIALOG_BOX_H
