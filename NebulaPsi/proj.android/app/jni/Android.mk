LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/NebulaGame.cpp \
                   $(LOCAL_PATH)/../../../Classes/Background.cpp \
                   $(LOCAL_PATH)/../../../Classes/BackgroundLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/Enemy.cpp \
                   $(LOCAL_PATH)/../../../Classes/Weapon.cpp \
                   $(LOCAL_PATH)/../../../Classes/MainMenu.cpp \
                   $(LOCAL_PATH)/../../../Classes/Asteroid.cpp \
                   $(LOCAL_PATH)/../../../Classes/Settings.cpp \
                   $(LOCAL_PATH)/../../../Classes/PublicVariables.cpp \
                   $(LOCAL_PATH)/../../../Classes/Utils.cpp \
                   $(LOCAL_PATH)/../../../Classes/GameOver.cpp \
                   $(LOCAL_PATH)/../../../Classes/PauseLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/HeadLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/ControlLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/StorageBox.cpp \
                   $(LOCAL_PATH)/../../../Classes/Assistant.cpp \
                   $(LOCAL_PATH)/../../../Classes/Loading.cpp \
                   $(LOCAL_PATH)/../../../Classes/PsiToast.cpp \
                   $(LOCAL_PATH)/../../../Classes/Gyroscope.cpp \
                   $(LOCAL_PATH)/../../../Classes/AddMob.cpp \
                   $(LOCAL_PATH)/../../../Classes/Explosions.cpp \
                   $(LOCAL_PATH)/../../../Classes/CheckNet.cpp \
                   $(LOCAL_PATH)/../../../Classes/AnimationTools.cpp \
                   $(LOCAL_PATH)/../../../Classes/Player.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
