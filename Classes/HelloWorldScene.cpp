/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "NLeaderboardRecordsFetchMessage.h"
#include "NTopicJoinMessage.h"
#include "NTopicMessageSendMessage.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

HelloWorld::~HelloWorld()
{
    delete m_client;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    m_label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (m_label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        m_label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - m_label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(m_label, 1);
    }

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }

    NLogger::getInstance().SetLevel(NLogLevel::Debug);

    m_client = &NClient::Builder("defaultkey")
        .Host("127.0.0.1")
        .Port(7350)
        .Build();

    auto loginFailedCallback = [this](const NError error)
    {
        CCLOGERROR("Login failed - error code %d, %s", error.GetErrorCode(), error.GetErrorMessage().c_str());

        if (error.GetErrorCode() == ErrorCode::AuthError)
        {
            registerDevice();
        }
    };

    CCLOG("Login...");
    m_client->Login(
        NAuthenticateMessage::Device(getDeviceId()),
        std::bind(&HelloWorld::onLoginSucceeded, this),
        loginFailedCallback);

    return true;
}

std::string HelloWorld::getDeviceId()
{
    return "pcdevice123456";
}

void HelloWorld::registerDevice()
{
    auto registerFailedCallback = [this](const NError error)
    {
        CCLOGERROR("Register failed - error code %d, %s", error.GetErrorCode(), error.GetErrorMessage().c_str());
    };

    CCLOG("Register...");
    m_client->Register(
        NAuthenticateMessage::Device(getDeviceId()),
        std::bind(&HelloWorld::onLoginSucceeded, this),
        registerFailedCallback);
}

void HelloWorld::onLoginSucceeded()
{
    CCLOG("Login succeeded. session id: %s", m_client->GetSession()->GetId().c_str());

    connect();
}

void HelloWorld::connect()
{
	CCLOG("Connect...");
	m_client->Connect([this]()
	{
		CCLOG("Connected");

		joinTopic("chat-room");
	});
}

void HelloWorld::joinTopic(const std::string& topicName)
{
	m_client->OnTopicMessage.push_back([this](const NTopicMessage& msg)
	{
		// msg.GetData() is JSON string
		CCLOG("OnTopicMessage %s", msg.GetData().c_str());
        m_label->setString(msg.GetUserId() + ": " + msg.GetData());
	});

	NTopicJoinMessage msg = NTopicJoinMessage::Builder()
		.TopicRoom(topicName)
		.Build();

	CCLOG("Joining room %s", topicName.c_str());

	m_client->Send(msg, [this](void* data)
	{
		NResultSet<NTopic>* result = (NResultSet<NTopic>*)data;

		auto& records = result->GetResults();

		if (records.size() > 0)
		{
			m_topic = records[0];

			CCLOG("Joined topic id %s", m_topic.GetTopicId().GetId().c_str());

			sendTopicMessage("Hey dude!");
		}

		delete result;

	}, [](NError error)
	{
		CCLOGERROR("JoinTopic failed - error code %d, %s", error.GetErrorCode(), error.GetErrorMessage().c_str());
	});
}

void HelloWorld::sendTopicMessage(const std::string& message)
{
	// data must be JSON
	std::string data = "{\"msg\":\"" + message + "\"}";
	NTopicMessageSendMessage msg = NTopicMessageSendMessage::Default(m_topic.GetTopicId(), data);

	CCLOG("sending topic message %s", message.c_str());

	m_client->Send(msg, [this](void* data)
	{
		NTopicMessageAck* ack = (NTopicMessageAck*)data;

		CCLOG("Sent OK. message id %s", ack->GetMessageId().c_str());

		delete ack;
	}, [](NError error)
	{
		CCLOGERROR("Send topic message failed - error code %d, %s", error.GetErrorCode(), error.GetErrorMessage().c_str());
	});
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
