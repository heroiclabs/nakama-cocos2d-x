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

#include "NWebSocket.h"
#include "nakama-cpp/log/NLogger.h"

USING_NS_CC;

namespace Nakama {

    NWebSocket::NWebSocket()
    {
    }

    NWebSocket::~NWebSocket()
    {
        disconnect();
    }

    void NWebSocket::connect(const std::string& url, NRtTransportType type)
    {
        _type = type;

        if (!_websocket.init(*this, url))
        {
            NLOG_ERROR("Unable to initialize websocket!");

            NRtTransportInterface::onError("Unable to initialize websocket");
        }
    }

    void NWebSocket::send(const NBytes & data)
    {
        if (_websocket.getReadyState() != network::WebSocket::State::OPEN)
        {
            NLOG_ERROR("Socket is not opened");
            return;
        }

        if (_type == NRtTransportType::Binary)
        {
            _websocket.send((const uint8_t*)data.data(), data.size());
        }
        else
        {
            _websocket.send(data);
        }
    }

    void NWebSocket::disconnect()
    {
        _websocket.closeAsync();
    }

    void NWebSocket::onOpen(network::WebSocket* ws)
    {
        onConnected();
    }

    void NWebSocket::onMessage(network::WebSocket* ws, const network::WebSocket::Data& data)
    {
        _receiveBuffer.assign(data.bytes, data.bytes + data.len);

        NRtTransportInterface::onMessage(_receiveBuffer);
    }

    void NWebSocket::onClose(network::WebSocket* ws)
    {
        onDisconnected();
    }

    void NWebSocket::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error)
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

        NRtTransportInterface::onError("websocket error: " + description);
    }

}
