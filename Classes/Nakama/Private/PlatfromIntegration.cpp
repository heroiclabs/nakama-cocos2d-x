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

#include "PlatfromIntegration.h"
#include "base/ccUtils.h"
#include "base/base64.h"
#include <sstream>

namespace Nakama {

using namespace std::chrono;

int64_t getCurrentTime()
{
    //return cocos2d::utils::getTimeInMilliseconds();

    milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    return ms.count();
}

std::string generateGuid()
{
    static uint32_t guid = 0;

    ++guid;

    std::stringstream ss;
    ss << getCurrentTime() << "_" << guid;

    return ss.str();
}

std::string base64(const void* binaryData, int len)
{
    std::string result;
    char* base64string = nullptr;

    cocos2d::base64Encode((const unsigned char*)binaryData, len, &base64string);

    result = base64string;

    free(base64string);

    return result;
}

Buffer unbase64(const char* ascii, int len)
{
    Buffer result;
    unsigned char* buffer = nullptr;
    int buffLen;

    buffLen = cocos2d::base64Decode((const unsigned char*)ascii, len, &buffer);

    result.assign(buffer, buffer + buffLen);

    free(buffer);

    return result;
}

std::string unbase64_to_str(const char* ascii, int len)
{
    std::string result;
    unsigned char* buffer = nullptr;
    int buffLen;

    buffLen = cocos2d::base64Decode((const unsigned char*)ascii, len, &buffer);

    result.assign(buffer, buffer + buffLen);

    free(buffer);

    return result;
}

}
