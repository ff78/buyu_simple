LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dlua_shared

LOCAL_MODULE_FILENAME := libcocos2dlua

# 遍历目录及子目录的函数
define walk
$(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

# 遍历Classes目录
ALLDATAFILES = $(call walk, $(LOCAL_PATH)/../../Classes/data)
LOGICFILES = $(call walk, $(LOCAL_PATH)/../../Classes/logic)
#SEVERFILES = $(call walk, $(LOCAL_PATH)/../../Classes/server)
SOUNDFILES = $(call walk, $(LOCAL_PATH)/../../Classes/sound)
UTILSFILES = $(call walk, $(LOCAL_PATH)/../../Classes/utils)
VIEWFILES = $(call walk, $(LOCAL_PATH)/../../Classes/view)

FILE_LIST := hellolua/main.cpp
FILE_LIST += ../../Classes/AppDelegate.cpp \
../../Classes/ide-support/SimpleConfigParser.cpp \
../../Classes/ide-support/RuntimeLuaImpl.cpp \
../../Classes/ide-support/lua_debugger.c \
../../Classes/server/ServerManager.cpp \
../../Classes/server/ShareHelper.cpp \
../../Classes/Cocos2dx/Common/CCUMSocialSDK.cpp \
../../Classes/Cocos2dx/Android/CCUMSocialController.cpp 

## 从所有文件中提取出所有.cpp文件
FILE_LIST += $(filter %.cpp, $(ALLDATAFILES))
FILE_LIST += $(filter %.cpp, $(LOGICFILES))
FILE_LIST += $(filter %.cpp, $(SEVERFILES))
FILE_LIST += $(filter %.cpp, $(SOUNDFILES))
FILE_LIST += $(filter %.cpp, $(UTILSFILES))
FILE_LIST += $(filter %.cpp, $(VIEWFILES))

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

#LOCAL_SRC_FILES := \
#../../Classes/AppDelegate.cpp \
#../../Classes/ide-support/SimpleConfigParser.cpp \
#../../Classes/ide-support/RuntimeLuaImpl.cpp \
#../../Classes/ide-support/lua_debugger.c \
#$(filter %.cpp, $(wildcard $(LOCAL_PATH)../../Classes/data)) \
#$(filter %.cpp, $(wildcard ../../Classes/logic)) \
#$(filter %.cpp, $(wildcard ../../Classes/server)) \
#$(filter %.cpp, $(wildcard ../../Classes/sound)) \
#$(filter %.cpp, $(wildcard ../../Classes/utils)) \
#$(filter %.cpp, $(wildcard ../../Classes/view)) \
#$(filter %.cpp, $(wildcard ../../Classes/view/node)) \
#$(filter %.cpp, $(wildcard ../../Classes/view/gameLayers)) \
#$(filter %.cpp, $(wildcard ../../Classes/view/scene)) \
#hellolua/main.cpp

#LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

#LOCAL_SRC_FILES := $(CPP_FILE_LIST:$(LOCAL_PATH)/%=%)
#LOCAL_SRC_FILES += $(C_FILE_LIST:$(LOCAL_PATH)/%=%)

#5 加入头文件
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../Share/Header \
$(LOCAL_PATH)/platform/android

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_STATIC_LIBRARIES := cocos2d_lua_static
LOCAL_STATIC_LIBRARIES += cocos2d_simulator_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,scripting/lua-bindings/proj.android)
$(call import-module,tools/simulator/libsimulator/proj.android)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
