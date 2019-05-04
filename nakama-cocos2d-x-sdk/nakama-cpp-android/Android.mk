#
# Copyright 2019 The Nakama Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

LIB_PATH := $(LOCAL_PATH)/../shared-libs/android/$(TARGET_ARCH_ABI)

include $(CLEAR_VARS)
LOCAL_MODULE := nakama-cpp
LOCAL_MODULE_FILENAME := lib$(LOCAL_MODULE)
LOCAL_SRC_FILES := $(LIB_PATH)/$(LOCAL_MODULE_FILENAME).so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_EXPORT_CFLAGS += -DNLOGS_ENABLED
include $(PREBUILT_SHARED_LIBRARY)
