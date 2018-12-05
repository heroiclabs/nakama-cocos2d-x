#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "NClient.h"

using namespace Nakama;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    ~HelloWorld();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

protected:
    std::string getDeviceId();
    void registerDevice();
    void onLoginSucceeded();
    void connect();
    void joinTopic(const std::string& topicName);
    void sendTopicMessage(const std::string& message);

private:
    NClient* m_client = nullptr;
    NTopic m_topic;
};

#endif // __HELLOWORLD_SCENE_H__
