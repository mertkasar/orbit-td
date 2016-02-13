LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../../Classes/AppDelegate.cpp \
                   ../../../Classes/Scenes/World.cpp \
                   ../../../Classes/Scenes/GameplayLayer.cpp \
                   ../../../Classes/Scenes/MapLayer.cpp \
                   ../../../Classes/Scenes/HUDLayer.cpp \
                   ../../../Classes/Scenes/MainMenuLayer.cpp \
                   ../../../Classes/Entities/EnemyShip.cpp \
                   ../../../Classes/Entities/Missile.cpp \
                   ../../../Classes/Entities/Explosion.cpp \
                   ../../../Classes/Entities/LaserBeam.cpp \
                   ../../../Classes/Entities/Bullet.cpp \
                   ../../../Classes/Entities/TowerMock.cpp \
                   ../../../Classes/Entities/WheelMenu.cpp \
                   ../../../Classes/Entities/DialogBox.cpp \
                   ../../../Classes/Entities/ResultPanel.cpp \
                   ../../../Classes/Entities/CostIndicator.cpp \
                   ../../../Classes/Entities/Turret.cpp \
                   ../../../Classes/Entities/MachineGun.cpp \
                   ../../../Classes/Entities/LaserGun.cpp \
                   ../../../Classes/Entities/MissileLauncher.cpp \
                   ../../../Classes/Entities/Planet.cpp \
                   ../../../Classes/Utilities/SteeringDirector.cpp \
                   ../../../Classes/Utilities/Path.cpp \
                   ../../../Classes/Utilities/Grid.cpp \
                   ../../../Classes/Utilities/Algorithm.cpp \
                   ../../../Classes/Utilities/Shake.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
