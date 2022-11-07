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

#include "NCocosHelper.h"
#include "nakama-cpp/NCocosLogSink.h"
#include "nakama-cpp/NCocosHTTP.h"
#include "nakama-cpp/NCocosWebSocket.h"

using namespace Nakama;


void NCocosHelper::init(NLogLevel logLevel)
{
    NLogger::init(std::make_shared<NCocosLogSink>(), logLevel);
}

NClientPtr NCocosHelper::createDefaultClient(const NClientParameters &parameters)
{
    return createRestClient(parameters, NHttpTransportPtr(new NCocosHTTP()));
}

NRtClientPtr NCocosHelper::createRtClient(const NClientPtr& client, int32_t port)
{
    NRtClientPtr rtClient;

    if (client)
    {
        rtClient = client->createRtClient(port, NRtTransportPtr(new NCocosWebSocket()));
    }

    return rtClient;
}

Nakama::NRtClientPtr NCocosHelper::createRtClient(const Nakama::NClientPtr &client,
                                                  const Nakama::RtClientParameters &parameters)
{
    NRtClientPtr rtClient;

    if (client)
    {
        rtClient = client->createRtClient(parameters, NRtTransportPtr(new NCocosWebSocket()));
    }

    return rtClient;
}
