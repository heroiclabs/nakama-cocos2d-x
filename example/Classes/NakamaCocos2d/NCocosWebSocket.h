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
 // Cocos2d wrappper for WebSocket
 //

#pragma once

#include "nakama-cpp/realtime/NRtTransportInterface.h"
#include "network/WebSocket.h"

namespace Nakama {

    class NCocosWebSocket : public cocos2d::network::WebSocket::Delegate, public NRtTransportInterface
    {
    public:
        NCocosWebSocket();
        ~NCocosWebSocket();

        void setActivityTimeout(uint32_t timeoutMs);
        uint32_t getActivityTimeout() const { return 0; }
        void tick() override {}
        void connect(const std::string& url, NRtTransportType type) override;
        void disconnect() override;
        bool send(const NBytes& data) override;

    private:
        // cocos2d::network::WebSocket::Delegate implementation
        void onOpen(cocos2d::network::WebSocket* ws) override;
        void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data) override;
        void onClose(cocos2d::network::WebSocket* ws) override;
        void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error) override;

        NBytes _receiveBuffer;
        NRtTransportType _type;
        cocos2d::network::WebSocket _websocket;
    };

}
