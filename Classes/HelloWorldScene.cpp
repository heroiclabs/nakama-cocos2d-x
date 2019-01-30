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
#include "NCocosLogSink.h"
#include "NWebSocket.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

HelloWorld::~HelloWorld()
{
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

    NLogger::init(std::make_shared<NCocosLogSink>(), NLogLevel::Debug);

    auto tickCallback = [this](float dt)
    {
        m_client->tick();

        if (m_rtClient)
            m_rtClient->tick();
    };

    getScheduler()->schedule(tickCallback, this, 0.05f /*sec*/, CC_REPEAT_FOREVER, 0, false, "nakama-tick");

    DefaultClientParameters parameters;

    parameters.host = "127.0.0.1";
    parameters.port = 7349;

    m_client = createDefaultClient(parameters);

    auto loginFailedCallback = [this](const NError error)
    {
        if (error.code == ErrorCode::NotFound)
        {
            registerDevice();
        }
    };

    CCLOG("Login...");
    m_client->authenticateDevice(
        getDeviceId(),
        opt::nullopt,
        false,
        std::bind(&HelloWorld::onLoginSucceeded, this, std::placeholders::_1),
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
    };

    CCLOG("Register...");
    m_client->authenticateDevice(
        getDeviceId(),
        opt::nullopt,
        true,
        std::bind(&HelloWorld::onLoginSucceeded, this, std::placeholders::_1),
        registerFailedCallback);
}

void HelloWorld::onLoginSucceeded(NSessionPtr session)
{
    m_session = session;

    CCLOG("Login succeeded. user id: %s", m_session->getUserId().c_str());

    connect();
}

void HelloWorld::connect()
{
    m_rtListener.reset(new NRtDefaultClientListener());

    m_rtListener->setConnectCallback([this]()
    {
        CCLOG("Connected!");
        joinChat("chat-room");
    });

    m_rtListener->setChannelMessageCallback([this](const NChannelMessage& msg)
    {
        // msg.GetData() is JSON string
        CCLOG("OnChannelMessage %s", msg.content.c_str());
        m_label->setString(msg.username + ": " + msg.content);
    });

    NRtTransportPtr transport(new NWebSocket());
    m_rtClient = m_client->createRtClient(7350, transport);
    m_rtClient->setListener(m_rtListener.get());

	CCLOG("Connect...");

    m_rtClient->connect(m_session, true/*, NRtClientProtocol::Json*/);
}

void HelloWorld::joinChat(const std::string& topicName)
{
    CCLOG("Joining room %s", topicName.c_str());

    m_rtClient->joinChat(
        topicName,
        NChannelType::ROOM,
        {},
        {},
        [this](NChannelPtr channel)
        {
            m_chatId = channel->id;

            CCLOG("Joined topic id %s", channel->id.c_str());

            sendChatMessage("Hey dude!");
        }, [](const NRtError& error)
        {
            CCLOGERROR("JoinTopic failed - error code %d, %s", error.code, error.message.c_str());
        }
    );
}

void HelloWorld::sendChatMessage(const std::string& message)
{
    // data must be JSON
    std::string data = "{\"msg\":\"" + message + "\"}";

    CCLOG("sending topic message %s", message.c_str());

    m_rtClient->writeChatMessage(m_chatId, data, [this](const NChannelMessageAck& ack)
    {
        CCLOG("Sent OK. message id %s", ack.message_id.c_str());
    },
    [](const NRtError& error)
    {
        CCLOGERROR("Send topic message failed - error code %d, %s", error.code, error.message.c_str());
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
