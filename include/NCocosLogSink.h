/*
 * Copyright 2019 The Nakama Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "nakama-cpp/log/NLogSinkInterface.h"
#include "cocos/cocos2d.h"

namespace Nakama {

    class NCocosLogSink : public NLogSinkInterface
    {
    public:
        ~NCocosLogSink() {}

        void log(NLogLevel level, const std::string& message, const char* func) override
        {
            switch (level)
            {
            case NLogLevel::Debug:
                CCLOG("%s: %s", func, message.c_str());
                break;
            case NLogLevel::Info:
                CCLOG("%s: %s", func, message.c_str());
                break;
            case NLogLevel::Warn:
                CCLOGWARN("%s: %s", func, message.c_str());
                break;
            case NLogLevel::Error:
                CCLOGERROR("%s: %s", func, message.c_str());
                break;
            case NLogLevel::Fatal:
                CCLOGERROR("%s: %s", func, message.c_str());
                break;
            }
        }

        void flush() override {}
    };
}
