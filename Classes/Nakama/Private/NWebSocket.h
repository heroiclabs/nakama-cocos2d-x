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

 //
 // Cocos2d wrappper for WebSocket
 //

#pragma once

#include "Defines.h"
#include "NLogger.h"
#include "network/WebSocket.h"

#include <functional>
#include <string>

namespace Nakama {

    class NWebSocket : public cocos2d::network::WebSocket::Delegate
    {

    public:
        // Initialize with given url
        NWebSocket(const std::string& host, const unsigned port, const std::string& path, const bool ssl);

        // clean up.
        ~NWebSocket();

        /************************************************************************/
        /* Set various callbacks for Socket Events                              */
        /************************************************************************/
        void SetConnectedCallBack(NConnUpdateCallback callBack) { ConnectedCallBack = callBack; }
        void SetErrorCallBack(NErrorCallback callBack) { ErrorCallBack = callBack; }
        void SetReceiveCallBack(NDataRcvdCallback callBack) { ReceivedCallBack = callBack; }
        void SetClosedCallBack(NConnUpdateCallback callBack) { ClosedCallBack = callBack; }

        /** Send raw data to remote end point. */
        bool Send(const uint8_t* Data, uint32_t Size);

        /** Send string to remote end point. */
        bool Send(const std::string& str);

        /** initiate the websocket connection using the currently set connection info and callbacks */
        void Connect();

        /** Close the websocket */
        void Close();

    private:
        // cocos2d::network::WebSocket::Delegate implementation
        void onOpen(cocos2d::network::WebSocket* ws) override;
        void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data) override;
        void onClose(cocos2d::network::WebSocket* ws) override;
        void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error) override;

        std::string Host;
        unsigned Port;
        std::string Path;
        bool UseSSL;

        /************************************************************************/
        /*	Various Socket callbacks											*/
        /************************************************************************/
        NConnUpdateCallback ConnectedCallBack;
        NErrorCallback ErrorCallBack;
        NDataRcvdCallback ReceivedCallBack;
        NConnUpdateCallback ClosedCallBack;

        /**  Recv Buffer */
        Buffer ReceivedBuffer;

        cocos2d::network::WebSocket websocket;
    };

}
