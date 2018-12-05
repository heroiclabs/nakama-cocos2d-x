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

#ifndef PLATFORM_MAC

#ifdef __UNREAL__
#include "Ssl.h"
#elif COCOS2D
#include "openssl/ssl.h"
#endif

#endif // PLATFORM_MAC

namespace Nakama {

	NWebSocket::NWebSocket(const std::string& host, const unsigned port, const std::string& path, const bool ssl) :
		Host(host),
		Port(port),
		Path(path),
		UseSSL(ssl)
	{
		isConnecting = false;

		SetLWSLogLevel();

		Protocols = new WebSocketInternalProtocol[3];
		memset(Protocols, 0, sizeof(WebSocketInternalProtocol) * 3);

		Protocols[0].name = "binary";
		Protocols[0].callback = websocket_callback;
		Protocols[0].per_session_data_size = 0;
		Protocols[0].rx_buffer_size = 10 * 1024 * 1024;

		Protocols[1].name = nullptr;
		Protocols[1].callback = nullptr;
		Protocols[1].per_session_data_size = 0;

		// Initialize our lws context
		struct lws_context_creation_info ContextInfo;
		memset(&ContextInfo, 0, sizeof ContextInfo);
		ContextInfo.port = CONTEXT_PORT_NO_LISTEN;
		ContextInfo.protocols = &Protocols[0];
		ContextInfo.gid = -1;
		ContextInfo.uid = -1;
		ContextInfo.user = this;
		Context = lws_create_context(&ContextInfo);
		assert(Context);
		//check(Context);
	}

	NWebSocket::~NWebSocket()
	{
		Flush();

		if (Context != nullptr) {
			// Clean up
			lws_context_destroy(Context);
			Context = nullptr;
		}
		if (Protocols) {
			delete []Protocols;
			Protocols = NULL;
		}
	}

	void NWebSocket::lwsLogger(int level, const char *line)
	{
		switch (level) {
		case LLL_ERR:
			NLogger::Format(Error, "websocket: %s", line);
			break;
		case LLL_WARN:
			NLogger::Format(Warn, "websocket: %s", line);
			break;
		case LLL_NOTICE:
		case LLL_INFO:
			NLogger::Format(Info, "websocket: %s", line);
			break;
		case LLL_DEBUG:
			NLogger::Format(Debug, "websocket: %s", line);
			break;
		}
	}

	void NWebSocket::SetLWSLogLevel() {
		lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_DEBUG | LLL_INFO, lwsLogger);
	}

	void NWebSocket::Connect()
	{
		isConnecting = true;
		if (Context == nullptr) {
			NLogger::Error("Could not connect: Context was null.");
			return;
		}
		if (LwsConnection != nullptr) {
			NLogger::Error("Could not connect: Already connected!");
			return;
		}

		// Setup our lws connection info
		struct lws_client_connect_info ConnectInfo;
		memset(&ConnectInfo, 0, sizeof(ConnectInfo));
		ConnectInfo.context = Context;
		ConnectInfo.address = Host.c_str();
		ConnectInfo.port = Port;
		ConnectInfo.path = Path.c_str();
		ConnectInfo.origin = NULL;
		ConnectInfo.protocol = Protocols[1].name;
		ConnectInfo.ietf_version_or_minus_one = -1;
		ConnectInfo.ssl_connection = UseSSL ? 1 : 0; // XXX: If you want to allow self-signed certs, change 1 to 2

		std::string fullhost;
		fullhost.append(Host).append(":").append(std::to_string(Port));
		ConnectInfo.host = fullhost.c_str();

		if (lws_client_connect_via_info(&ConnectInfo) == nullptr) {
			NLogger::Error("Unable to initialize connection!");
			return;
		}
	}

	bool NWebSocket::Send(uint8_t* Data, uint32_t Size)
	{
		Buffer buffer(LWS_PRE); // Reserve space for WS header data

		buffer.insert(buffer.end(), Data, Data + Size);

		OutgoingBuffer.push_back(buffer);

		return true;
	}

	void NWebSocket::Tick()
	{
		HandlePacket();
	}

	void NWebSocket::HandlePacket()
	{
		if (Context) {
			lws_service(Context, 0);
			lws_callback_on_writable_all_protocol(Context, &Protocols[0]);
		}
	}

	void NWebSocket::Flush()
	{
		auto PendingMesssages = OutgoingBuffer.size();

		while (OutgoingBuffer.size() > 0)
		{
			if (Protocols)
			{
				lws_callback_on_writable_all_protocol(Context, &Protocols[0]);
			}
			else
			{
				lws_callback_on_writable(LwsConnection);
			}
			HandlePacket();

			if (PendingMesssages >= OutgoingBuffer.size())
			{
				NLogger::Warn("Unable to flush all of OutgoingBuffer in FWebSocket.");
				break;
			}
		};
	}

	void NWebSocket::Close()
	{
		lws_close_reason(LwsConnection, lws_close_status::LWS_CLOSE_STATUS_NORMAL, NULL, 0);
	}

	void NWebSocket::OnRawReceive(void* Data, uint32_t Size, uint32_t Remaining)
	{
		auto charData = (const uint8_t*)Data;
		ReceivedBuffer.insert(ReceivedBuffer.end(), charData, charData + (Size * sizeof(uint8_t))); // consumes all of Data
		if (Remaining == 0)
		{
			if (ReceivedCallBack) ReceivedCallBack(ReceivedBuffer);
			ReceivedBuffer.clear();
		}
	}

	void NWebSocket::OnWebSocketWritable()
	{
		if (LwsConnection == nullptr)
		{
			return;
		}

		if (OutgoingBuffer.size() == 0)
			return;

		Buffer& Packet = OutgoingBuffer.front();

		uint32_t TotalDataSize = Packet.size() - LWS_PRE;
		uint32_t DataToSend = TotalDataSize;
		// TODO: We could break this up to send over multiple ticks instead.
		while (DataToSend)
		{
			int Sent = lws_write(LwsConnection, Packet.data() + LWS_PRE + (DataToSend - TotalDataSize), DataToSend, (lws_write_protocol)LWS_WRITE_BINARY);
			if (Sent < 0)
			{
				if (ErrorCallBack) ErrorCallBack("Fatal error on lws_write.");
				// Clean up
				lws_context_destroy(Context);
				Context = nullptr;
				return;
			}
			if ((uint32_t)Sent < DataToSend)
			{
				NLogger::Format(Warn, "Could not write all '%d' bytes to socket", DataToSend);
			}
			DataToSend -= Sent;
		}

		OutgoingBuffer.pop_front();

		if (OutgoingBuffer.size() > 0) {
			lws_callback_on_writable(LwsConnection);
		}
	}

	int NWebSocket::websocket_callback(
		WebSocketInternal *Instance,
		WebSocketInternalCallback Reason,
		void *User,
		void *In,
		size_t Len)
	{
		WebSocketInternalContext *Context = lws_get_context(Instance);
		NWebSocket* Self = (NWebSocket*)lws_context_user(Context);

		switch (Reason)
		{
		case LWS_CALLBACK_PROTOCOL_INIT:
		case LWS_CALLBACK_PROTOCOL_DESTROY:
		case LWS_CALLBACK_GET_THREAD_ID:
		case LWS_CALLBACK_LOCK_POLL:
		case LWS_CALLBACK_UNLOCK_POLL:
		case LWS_CALLBACK_ADD_POLL_FD:
		case LWS_CALLBACK_DEL_POLL_FD:
		case LWS_CALLBACK_CHANGE_MODE_POLL_FD:
		case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
		case LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH:
		case LWS_CALLBACK_RECEIVE_PONG:
			// Ignored!
			break;

		case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS:
		case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS:
		{
			// XXX: SSL not currently supported on Mac - HTTP and SSL modules collide with each other.
			// XXX: Could maybe support this by direct use of OpenSSL?
#ifndef PLATFORM_MAC
			//SSL_CTX* SslContext = reinterpret_cast<SSL_CTX*>(User);
			//FSslModule::Get().GetCertificateManager().AddCertificatesToSslContext(SslContext);
#endif
			break;
		}
        case LWS_CALLBACK_WSI_CREATE:
            NLogger::Trace("WebSocket->Connection Created.");
            break;
		case LWS_CALLBACK_CLIENT_ESTABLISHED:
		{
			NLogger::Trace("WebSocket->Connection Established.");
			Self->isConnecting = false;
			Self->LwsConnection = Instance;
			if (Self->OutgoingBuffer.size() != 0)
			{
				lws_callback_on_writable(Self->LwsConnection);
			}
			if (Self->ConnectedCallBack) Self->ConnectedCallBack();
		}
		break;
		case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		{
			std::string msg = In && Len > 0 ? std::string((char *)In, Len) : "";
			NLogger::Format(Error, "WebSocket->Client Connection Error: %s", msg.c_str());
			if (Self->ErrorCallBack) Self->ErrorCallBack(msg);
			return -1;
		}
		break;
		case LWS_CALLBACK_CLIENT_RECEIVE:
		{
			NLogger::Trace("WebSocket->Connection Receive.");
			auto bytesLeft = lws_remaining_packet_payload(Instance);
			Self->OnRawReceive(In, (uint32_t)Len, (uint32_t)bytesLeft);
		}
		case LWS_CALLBACK_CLIENT_WRITEABLE:
		case LWS_CALLBACK_SERVER_WRITEABLE:
		{
			Self->OnWebSocketWritable();
			break;
		}
		case LWS_CALLBACK_CLOSED:
		{
			NLogger::Trace("WebSocket->Connection Closed.");
			if (Self->ClosedCallBack) Self->ClosedCallBack();
			return -1;
		}
		case LWS_CALLBACK_WSI_DESTROY:
		{
			NLogger::Trace("WebSocket->Connection Destroyed.");
			break;
		}
		default:
			NLogger::Format(Warn, "WebSocket->Connection Received unhandled message: %d", (int)Reason);
			break;
		}

		return 0;
	}
}
