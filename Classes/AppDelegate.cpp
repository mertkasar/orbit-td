#include <AppDelegate.h>

#include <audio/include/SimpleAudioEngine.h>
#include "Scenes/SplashScene.h"

USING_NS_CC;

const Size RESOLUTION = Size(1280, 720);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() {
}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages() {
    return 0;
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::create("Orbit TD");
        glview->setFrameSize(RESOLUTION.width, RESOLUTION.height);
        director->setOpenGLView(glview);
    }

    //director->setDisplayStats(true);

    director->setAnimationInterval(1.f / 60.f);

    glview->setDesignResolutionSize(RESOLUTION.width, RESOLUTION.height, ResolutionPolicy::EXACT_FIT);

    register_all_packages();

    auto scene = SplashScene::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
