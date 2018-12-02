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

#include <stdint.h>
#include <string>
#include "Defines.h"

namespace Nakama {

    // get current time since epoch in milliseconds
    int64_t getCurrentTime();

    std::string generateGuid();

    // Converts binary data to base64 string.
    std::string base64(const void* binaryData, int len);

    // Converts base64 string to binary data.
    Buffer unbase64(const char* ascii, int len);
    std::string unbase64_to_str(const char* ascii, int len);
}
