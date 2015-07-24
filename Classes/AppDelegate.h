#ifndef  APP_DELEGATE_H
#define  APP_DELEGATE_H

#include <cocos2d.h>

class AppDelegate : private cocos2d::Application {
public:
    AppDelegate();

    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    virtual bool applicationDidFinishLaunching();

    virtual void applicationDidEnterBackground();

    virtual void applicationWillEnterForeground();
};

#endif // APP_DELEGATE_H

