#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "NLeaderboardRecordsFetchMessage.h"

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

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
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

    m_client = &NClient::Builder("<Server key>")
        .Host("127.0.0.1")
        .Port(7350)
        .Build();

    auto loginFailedCallback = [](const NError error)
    {
        CCLOGERROR("Login failed - error code %d, %s", error.GetErrorCode(), error.GetErrorMessage().c_str());
    };

    CCLOGINFO("Login...");
    m_client->Login(
        NAuthenticateMessage::Email("test@google.com", "123"),
        std::bind(&HelloWorld::onLoginSucceeded, this, std::placeholders::_1),
        loginFailedCallback);

    return true;
}

void HelloWorld::onLoginSucceeded(NSession* session)
{
    CCLOGINFO("Login succeeded - session id %s", session->GetId().c_str());

    CCLOGINFO("Connect...");
    m_client->Connect(session, [this]()
    {
        CCLOGINFO("Connected");

        NLeaderboardRecordsFetchMessage msg = NLeaderboardRecordsFetchMessage::Builder("<leaderboard_id>")
            .Limit(10)
            .Build();

        m_client->Send(msg, [](void* data)
        {
            NResultSet<Nakama::NLeaderboardRecord>* result = (NResultSet<Nakama::NLeaderboardRecord>*)data;

            auto records = result->GetResults();

            delete result;

        }, [](NError error)
        {
            CCLOGERROR("LeaderboardsList failed - error code %d, %s", error.GetErrorCode(), error.GetErrorMessage().c_str());
        });
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
