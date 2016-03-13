#include "SplashScene.h"

#include "World.h"

#include <2d/CCSprite.h>
#include <2d/CCActionInterval.h>
#include <2d/CCActionInstant.h>
#include <base/CCDirector.h>
#include <platform/CCFileUtils.h>
#include <2d/CCSpriteFrameCache.h>
#include <audio/include/SimpleAudioEngine.h>
#include <ui/UIText.h>
#include <renderer/CCTextureCache.h>

USING_NS_CC;

SplashScene::SplashScene() {
    CCLOG("SplashScene created");
}

SplashScene::~SplashScene() {
    CCLOG("SplashScene deleted");
}

Scene *SplashScene::createScene() {
    auto scene = Scene::create();

    auto layer = SplashScene::create();
    scene->addChild(layer);

    return scene;
}

bool SplashScene::init() {
    if (!LayerColor::initWithColor(Color4B::BLACK)) {
        return false;
    }

    setCascadeOpacityEnabled(true);
    setOpacity(0);

    auto sprite = Sprite::create("textures/logo.png");
    sprite->setPosition(640, 360);
    addChild(sprite);

    auto poweredBy = Node::create();
    poweredBy->setCascadeOpacityEnabled(true);
    poweredBy->setPosition(sprite->getPosition() - Vec2(-20, 120.f));

    auto label = ui::Text::create("Powered by:", "fonts/kenvector_future.ttf", 20);
    label->setPosition(Vec2(-70.f, 0.f));
    poweredBy->addChild(label);

    sprite = Sprite::create("textures/cocos2dx.png");
    sprite->setPosition(Vec2(85.f, 2.f));
    poweredBy->addChild(sprite);

    addChild(poweredBy);

    runAction(Sequence::create(FadeIn::create(1.f),
                               CallFunc::create(CC_CALLBACK_0(SplashScene::loadResources, this)),
                               FadeOut::create(1.f),
                               CallFunc::create([&]() {
                                   auto scene = World::createScene();
                                   Director::getInstance()->replaceScene(scene);
                               }), NULL));

    return true;
}

void SplashScene::loadResources() {
    CCLOG("Loading resources");

    auto index = FileUtils::getInstance()->getValueMapFromFile("index.plist");
    auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
    auto spriteCache = SpriteFrameCache::getInstance();
    auto textureCache = Director::getInstance()->getTextureCache();

    auto audio = index.at("audio").asValueMap();
    for (auto bg : audio.at("background").asValueVector()) {
        audioEngine->preloadBackgroundMusic(bg.asString().c_str());
    }

    for (auto effect : audio.at("effect").asValueVector()) {
        audioEngine->preloadEffect(effect.asString().c_str());
    }

    for (auto sheet : index.at("spritesheet").asValueVector()) {
        spriteCache->addSpriteFramesWithFile(sheet.asString());
    }

    for (auto texture : index.at("textures").asValueVector()) {
        textureCache->addImage(texture.asString());
    }
}
