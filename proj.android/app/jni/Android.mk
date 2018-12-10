LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libprotobuf
LOCAL_MODULE_FILENAME := libprotobuf
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../Classes/ThirdParty/lib/android/$(TARGET_ARCH_ABI)/libprotobuf.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/HelloWorldScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NCocosTransport.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NWebSocket.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/PlatfromIntegration.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NGroupsFetchMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NSelfUnlinkMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NMatchCreateMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NGroupRemoveMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NMatchmakeRemoveMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NStorageUpdateMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NFriendRemoveMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NFriendBlockMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NPurchaseValidateMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NNotificationsListMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NLeaderboardsListMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NClient.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NGroupPromoteUserMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NNotificationsRemoveMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NMatchLeaveMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NLeaderboardRecordsFetchMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NTopicMessageSendMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NRuntimeRpcMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NStorageWriteMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NSelfUpdateMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NMatchDataSendMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NFriendsListMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NStorageFetchMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NLeaderboardRecordsListMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NGroupUsersListMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NSelfFetchMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NFriendAddMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NUsersFetchMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NSession.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NGroupsSelfListMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NLogger.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NGroupJoinMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NCursor.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NStorageListMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/api.pb.cc \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NSelf.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NMatchmakeAddMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NFriend.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NGroupAddUserMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NMatchJoinMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NTopicLeaveMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NTopicMessagesListMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NGroupCreateMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NTopicJoinMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NGroupUpdateMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NGroupsListMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NAuthenticateMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NSelfLinkMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NGroupLeaveMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NLeaderboardRecordWriteMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NGroupKickUserMessage.cpp \
                   $(LOCAL_PATH)/../../../Classes/Nakama/Private/NakamaSDK/NStorageRemoveMessage.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Classes/Nakama/Public/NakamaSDK
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Classes/Nakama/Public
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Classes/Nakama/Private
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Classes/Nakama/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Classes/ThirdParty/include

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_CFLAGS += -DNAKAMA_API= -DCOCOS2D

LOCAL_STATIC_LIBRARIES := cocos2dx_static
#LOCAL_STATIC_LIBRARIES += libprotobuf
LOCAL_WHOLE_STATIC_LIBRARIES := libprotobuf

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
