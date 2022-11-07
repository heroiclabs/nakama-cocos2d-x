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

#include "nakama-cpp/NCocosHTTP.h"
#include "network/HttpClient.h"
#include "nakama-cpp/StrUtil.h"

USING_NS_CC;

namespace Nakama {

    using namespace cocos2d::network;

    NCocosHTTP::NCocosHTTP()
    {
    }

    NCocosHTTP::~NCocosHTTP()
    {
    }

    void NCocosHTTP::setBaseUri(const std::string & uri)
    {
        _baseUri = uri;
    }

    void NCocosHTTP::request(const NHttpRequest & req, const NHttpResponseCallback & callback)
    {
        HttpRequest* cocosReq = new HttpRequest();

        if (!req.headers.empty())
        {
            std::vector<std::string> headers;

            for (auto it : req.headers)
            {
                headers.push_back(it.first + ":" + it.second);
            }

            cocosReq->setHeaders(headers);
        }

        std::string url(_baseUri + req.path);
        bool firstArg = true;

        for (auto& it : req.queryArgs)
        {
            if (firstArg)
            {
                url += '?';
                firstArg = false;
            }
            else
            {
                url += '&';
            }
            url += it.first;
            url += '=';
            url += urlEncode(it.second);
        }

        cocosReq->setUrl(url);
        cocosReq->setRequestData(req.body.c_str(), req.body.size());

        HttpRequest::Type type;

        switch (req.method)
        {
        case NHttpReqMethod::POST: type = HttpRequest::Type::POST; break;
        case NHttpReqMethod::PUT : type = HttpRequest::Type::PUT; break;
        case NHttpReqMethod::GET : type = HttpRequest::Type::GET; break;
        case NHttpReqMethod::DEL : type = HttpRequest::Type::DELETE; break;
        default: type = HttpRequest::Type::UNKNOWN; break;
        }

        cocosReq->setRequestType(type);

        cocosReq->setResponseCallback([callback](HttpClient* client, HttpResponse* response)
        {
            if (callback)
            {
                NHttpResponsePtr responsePtr(new NHttpResponse());

                responsePtr->statusCode = response->getResponseCode();
                responsePtr->body.assign(response->getResponseData()->data(), response->getResponseData()->data() + response->getResponseData()->size());
                responsePtr->errorMessage = response->getErrorBuffer();

                callback(responsePtr);
            }
        });

        HttpClient::getInstance()->send(cocosReq);
        cocosReq->release();
    }

    void NCocosHTTP::cancelAllRequests()
    {
        HttpClient::getInstance()->destroyInstance();
    }

}
