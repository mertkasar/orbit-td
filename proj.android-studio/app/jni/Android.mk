LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../../Classes/AppDelegate.cpp \
                   ../../../Classes/Scenes/GameScene.cpp \
                   ../../../Classes/Scenes/WaveDirector.cpp \
                   ../../../Classes/Entities/Creeps/Creep.cpp \
                   ../../../Classes/Entities/Missile.cpp \
                   ../../../Classes/Entities/Towers/Tower.cpp \
                   ../../../Classes/Entities/Towers/Turret.cpp \
                   ../../../Classes/Entities/Towers/Laser.cpp \
                   ../../../Classes/Entities/Towers/RLauncher.cpp \
                   ../../../Classes/Utilities/SteeringDirector.cpp \
                   ../../../Classes/Utilities/Path.cpp \
                   ../../../Classes/Utilities/Grid.cpp \
                   ../../../Classes/Utilities/Algorithm.cpp \
                   ../../../Classes/UI/WheelMenu.cpp \
                   ../../../Classes/UI/HUD.cpp \

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
