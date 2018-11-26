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

#include "NCocosTransport.h"
#include "NCocosLogSink.h"
#include "NWebSocket.h"
#include "network/HttpClient.h"

namespace Nakama {
	using namespace cocos2d::network;

	NCocosTransport::~NCocosTransport()
	{
		delete socket;
		socket = nullptr;
	}

	void NCocosTransport::Post(std::string uri, AuthenticateRequest* payload, std::string authHeader, std::string langHeader, unsigned timeout, unsigned connectTimeout,
		const std::function<void(const std::vector<uint8_t> &)> &successAction,
		const std::function<void(const int32_t &)> &errorAction)
	{
		NLogger::Format(Info, "Nakama::NTransport->Post() - '%s'", uri.c_str());

		HttpRequest* httpRequest = new HttpRequest();
		std::vector<std::string> headers;

		httpRequest->setUrl(uri);
		httpRequest->setRequestType(HttpRequest::Type::POST);

		// Add Headers
		headers.push_back("Content-Type: application/octet-stream;");
		headers.push_back("Accept: application/octet-stream;");
		std::string agent = "nakama-cocossdk/";
		agent += cocos2d::cocos2dVersion();
		headers.push_back("User-Agent: " + agent);
		headers.push_back("Authorization: " + authHeader);
		headers.push_back("Accept-Language: " + langHeader);

		httpRequest->setHeaders(headers);

		// Pack payload
		std::string serializedPayload = payload->SerializeAsString();
		httpRequest->setRequestData(serializedPayload.c_str(), serializedPayload.size());

		// Set up callback for response
		httpRequest->setResponseCallback(
			[successAction, errorAction](HttpClient* client, HttpResponse* response) {
			if (response->isSucceed())
			{
				if (successAction) {
					// alas, we have to convert std::vector<char> to a std::vector<uint8_t> :(
					std::vector<char>* content = response->getResponseData();
					size_t contentLength = content->size();
					std::vector<uint8_t> rVal(contentLength);
					for (size_t i = 0; i < contentLength; i++)
					{
						rVal[i] = (*content)[i];
					}
					successAction(rVal);
				}
			}
			else
			{
				if (errorAction) errorAction(response->getResponseCode());
			}
		});

		// Kick off processing
		cocos2d::network::HttpClient::getInstance()->send(httpRequest);
	}

	void NCocosTransport::CreateWebSocket(const std::string host, const unsigned port, const std::string& path, const bool ssl)
	{
		socket = new NWebSocket(host, port, path, ssl);

		// OnConnect callback
		socket->SetConnectedCallBack([=]() { 
			if (ConnectCallback) ConnectCallback(); 
		});

		// OnClosed callback + cleanup
		socket->SetClosedCallBack([=]() { 
			// Release socket handle
				delete socket;
				socket = nullptr;

			NLogger::Trace("Socket Closed.");
			if (CloseCallback) CloseCallback(); 
		});

		// OnError callback
		socket->SetErrorCallBack([=](const std::string &msg) { if (ErrorCallback) ErrorCallback(msg); });
		
		// OnMessageReceived callback
		socket->SetReceiveCallBack([=](const std::vector<uint8_t> data) {
			if (MessageReceiveCallback) MessageReceiveCallback(data); 
		});

		// Now that we're all setup, connect!
		socket->Connect();
	}

	void NCocosTransport::Tick(float DeltaTime) {
		if (socket != nullptr) {
			socket->Tick();
		}
	}

	void NCocosTransport::Connect(const std::string host, const unsigned port, const std::string& path, const bool ssl)
	{
		if (socket == nullptr) {
			CreateWebSocket(host, port, path, ssl);
		}
	}

	void NCocosTransport::Close()
	{
		if (socket) {
			socket->Close();
		}
	}

	void NCocosTransport::Send(std::string data, std::function<void(bool)> callback)
	{
		if (socket) {
			bool success = socket->Send((uint8_t*)data.data(), data.size());
			if (callback) callback(success);
		}
		else {
			if (callback) callback(false);
		}
	}
}
