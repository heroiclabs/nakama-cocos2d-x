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

#pragma once

#include "INTransport.h"

namespace Nakama {

	class NWebSocket;

	class NAKAMA_API NCocosTransport : public INTransport
	{
	public:
		NCocosTransport() {}
		~NCocosTransport();

		// Inherited via INTransport
		virtual void Post(
			std::string uri,
			AuthenticateRequest* payload,
			std::string authHeader,
			std::string langHeader,
			unsigned timeout,
			unsigned connectTimeout,
			const std::function<void(const std::vector<uint8_t> &)> &successAction,
			const std::function<void(const int32_t &)> &errorAction) override;
		virtual void Connect(const std::string host, const unsigned port, const std::string& path, const bool ssl) override;
		virtual void Close() override;
		virtual void Send(const std::string& data, std::function<void(bool)> callback = nullptr) override;
		virtual void Tick(float DeltaTime) override;

	private:
		NWebSocket* socket = nullptr;
		void CreateWebSocket(const std::string host, const unsigned port, const std::string& path, const bool ssl);
	};
}
