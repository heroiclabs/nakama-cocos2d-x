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

 //
 // Cocos2d wrappper for HTTP transport
 //

#pragma once

#include "nakama-cpp/NHttpTransportInterface.h"

namespace Nakama {

    class NCocosHTTP : public NHttpTransportInterface
    {
    public:
        NCocosHTTP();
        ~NCocosHTTP();

        void setBaseUri(const std::string& uri) override;

        void tick() override {}

        void request(const NHttpRequest& req, const NHttpResponseCallback& callback = nullptr) override;

    private:
        std::string _baseUri;
    };

}
