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
// Wrapper around libwebsocket
//

#pragma once

#include "Defines.h"
#include "NLogger.h"

#include <functional>
#include <string>
#include <vector>
#include <list>

#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h"
#endif

#define UI UI_ST
#ifndef LWS_INCLUDED
    #if COCOS2D
        #define LWS_DLL
    #endif
    #include "libwebsockets.h"
    #define LWS_INCLUDED
#endif // !LWS_INCLUDED
#undef UI

#if PLATFORM_WINDOWS
#include "HideWindowsPlatformTypes.h"
#endif

typedef struct lws_context				WebSocketInternalContext;
typedef struct lws						WebSocketInternal;
typedef struct lws_protocols			WebSocketInternalProtocol;
typedef enum lws_callback_reasons		WebSocketInternalCallback;

namespace Nakama {

	class NWebSocket
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
		bool Send(uint8_t* Data, uint32_t Size);

		/** service libwebsocket.			   */
		void Tick();

		/** service libwebsocket until outgoing buffer is empty */
		void Flush();

		/** initiate the websocket connection using the currently set connection info and callbacks */
		void Connect();

		/** Close the websocket                */
		void Close();

	private:
		static void lwsLogger(int level, const char *line);

		bool isConnecting;

		void HandlePacket();
		void OnRawReceive(void* Data, uint32_t Size, uint32_t Remaining);
		void OnWebSocketWritable();
		void SetLWSLogLevel();

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

		/**  Recv and Send Buffers, serviced during the Tick */
		Buffer ReceivedBuffer;
		std::list<Buffer> OutgoingBuffer;

		/** libwebsocket internal context*/
		WebSocketInternalContext* Context = nullptr;

		/** libwebsocket web socket */
		WebSocketInternal* LwsConnection = nullptr;

		/** libwebsocket Protocols that can be serviced by this implemenation*/
		WebSocketInternalProtocol* Protocols = nullptr;

		static int websocket_callback(WebSocketInternal *Instance, WebSocketInternalCallback reason, void *user, void *in, size_t len);
	};

}
