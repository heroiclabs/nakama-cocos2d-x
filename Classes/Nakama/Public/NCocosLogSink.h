/**
* Copyright 2017 The Nakama Authors
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once

#include "Defines.h"
#include "INLogSink.h"
#include "NLogLevel.h"
#include "cocos2d.h"

namespace Nakama {

	class NAKAMA_API NCocosLogSink : public INLogSink {

	public:
		~NCocosLogSink() {}

		void Log(const NLogMessage& msg) override {
			switch (msg.level) {
			case NLogLevel::Trace:
				CCLOG("%s", msg.message.c_str());
				break;
			case NLogLevel::Debug:
				CCLOG("%s", msg.message.c_str());
				break;
			case NLogLevel::Info:
				CCLOG("%s", msg.message.c_str());
				break;
			case NLogLevel::Warn:
				CCLOGWARN("%s", msg.message.c_str());
				break;
			case NLogLevel::Error:
				CCLOGERROR("%s", msg.message.c_str());
				break;
			case NLogLevel::Fatal:
				CCLOGERROR("%s", msg.message.c_str());
				break;
			}
		}

		void Flush() override {}

	private:
	};
}
