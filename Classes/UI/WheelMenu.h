#ifndef WHEELMENU_H
#define WHEELMENU_H

#include <math/Vec2.h>

namespace cocos2d {
    class Menu;

    class Layer;

    namespace ui {
        class Layout;
    }
}

class WheelMenu {
private:
    cocos2d::ui::Layout *mRoot;

public:
    void init(cocos2d::Layer *pLayer);

    void openAt(cocos2d::Vec2 pPosition);

    void close();
};

#endif //WHEELMENU_H
