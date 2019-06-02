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

#include "NakamaCocos2d/NCocosWebSocket.h"
#include "nakama-cpp/log/NLogger.h"

#undef NMODULE_NAME
#define NMODULE_NAME "NCocosWebSocket"

USING_NS_CC;

namespace Nakama {

    NCocosWebSocket::NCocosWebSocket()
    {
    }

    NCocosWebSocket::~NCocosWebSocket()
    {
        disconnect();
    }

    void NCocosWebSocket::setActivityTimeout(uint32_t timeoutMs)
    {
        if (timeoutMs > 0)
        {
            NLOG_ERROR("Not supported");
        }
    }

    void NCocosWebSocket::connect(const std::string& url, NRtTransportType type)
    {
        _type = type;

        if (!_websocket.init(*this, url))
        {
            NLOG_ERROR("Unable to initialize websocket!");

            fireOnError("Unable to initialize websocket");
        }
    }

    bool NCocosWebSocket::send(const NBytes & data)
    {
        if (_websocket.getReadyState() != network::WebSocket::State::OPEN)
        {
            NLOG_ERROR("Socket is not opened");
            return false;
        }

        if (_type == NRtTransportType::Binary)
        {
            _websocket.send((const uint8_t*)data.data(), (unsigned int)data.size());
        }
        else
        {
            _websocket.send(data);
        }

        return true;
    }

    void NCocosWebSocket::disconnect()
    {
        _websocket.closeAsync();
    }

    void NCocosWebSocket::onOpen(network::WebSocket* ws)
    {
        fireOnConnected();
    }

    void NCocosWebSocket::onMessage(network::WebSocket* ws, const network::WebSocket::Data& data)
    {
        _receiveBuffer.assign(data.bytes, data.bytes + data.len);

        fireOnMessage(_receiveBuffer);
    }

    void NCocosWebSocket::onClose(network::WebSocket* ws)
    {
        NRtClientDisconnectInfo info;

        fireOnDisconnected(info);
    }

    void NCocosWebSocket::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error)
    {
        std::string description;

        switch (error)
        {
        case network::WebSocket::ErrorCode::CONNECTION_FAILURE: description = "CONNECTION_FAILURE"; break;
        case network::WebSocket::ErrorCode::TIME_OUT          : description = "TIME_OUT"; break;
        case network::WebSocket::ErrorCode::UNKNOWN           : description = "UNKNOWN"; break;

        default:
            break;
        }

        fireOnError("websocket error: " + description);
    }

}
