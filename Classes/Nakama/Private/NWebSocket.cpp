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

#include "NWebSocket.h"

namespace Nakama {

    NWebSocket::NWebSocket(const std::string& host, const unsigned port, const std::string& path, const bool ssl) :
        Host(host),
        Port(port),
        Path(path),
        UseSSL(ssl)
    {
    }

    NWebSocket::~NWebSocket()
    {
        Close();
    }

    void NWebSocket::Connect()
    {
        std::string fullhost;

        if (UseSSL)
            fullhost.append("wss://");
        else
            fullhost.append("ws://");

        fullhost.append(Host).append(":").append(std::to_string(Port)).append(Path);

        std::vector<std::string> protocols = { "binary" };

        if (!websocket.init(*this, fullhost, &protocols)) {
            NLogger::Error("NWebSocket->Connect - Unable to initialize websocket!");

            if (ErrorCallBack)
                ErrorCallBack("Unable to initialize websocket");
        }
    }

    bool NWebSocket::Send(const uint8_t* Data, uint32_t Size)
    {
        if (websocket.getReadyState() != cocos2d::network::WebSocket::State::OPEN)
        {
            NLogger::Error("NWebSocket->Send - Socket is not opened");
            return false;
        }

        websocket.send(Data, Size);
        return true;
    }

    bool NWebSocket::Send(const std::string& str)
    {
        return Send((const uint8_t*)str.c_str(), str.size());
    }

    void NWebSocket::Close()
    {
        websocket.closeAsync();
    }

    void NWebSocket::onOpen(cocos2d::network::WebSocket* ws)
    {
        NLogger::Info("NWebSocket->onOpen - Socket connected");

        if (ConnectedCallBack)
            ConnectedCallBack();
    }

    void NWebSocket::onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data)
    {
        ReceivedBuffer.assign(data.bytes, data.bytes + data.len);
        //NLogger::Info("NWebSocket->onMessage: " + std::string(data.bytes, data.bytes + data.len));

        if (ReceivedCallBack)
            ReceivedCallBack(ReceivedBuffer);
    }

    void NWebSocket::onClose(cocos2d::network::WebSocket* ws)
    {
        NLogger::Info("NWebSocket->onClose");

        if (ClosedCallBack)
            ClosedCallBack();
    }

    void NWebSocket::onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error)
    {
        NLogger::Error("NWebSocket->onError " + std::to_string((int)error));

        if (ErrorCallBack)
            ErrorCallBack("websocket error");
    }

}
