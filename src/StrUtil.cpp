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

#include <sstream>
#include "nakama-cpp-cocos2d-x/StrUtil.h"
#include <google/protobuf/stubs/strutil.h>

#include <regex>

namespace Nakama {

using namespace std;

std::string encodeURIComponent(std::string decoded)
{
    std::ostringstream oss;
    std::regex r("[-.0-9A-Za-z_~]");

    for (char c : decoded)
    {
        if (std::regex_match(std::string(1, c), r))
        {
            oss << c;
        }
        else
        {
            oss <<'%' << std::uppercase << std::hex << static_cast<uint16_t>(0xff & c) ;
        }
    }
    return oss.str();
}


} // namespace Nakama
