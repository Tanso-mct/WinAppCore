#include "WinAppCore_Test/pch.h"
#include "WinAppCore/include/WACore.h"
#pragma comment(lib, "WinAppCore.lib")

class Event
{
public:
    Event() = default;
    virtual ~Event() = default;

    void OnCall(bool& isCalled)
    {
        std::cout << "Event is called" << std::endl;
        isCalled = true;
    }
};

TEST(WinAppCore_event, CallEvent)
{
    // EVENT_KEY -> std::string
    // FUNC_KEY -> int
    // EVENT -> Event
    // Args -> bool&
    WACore::EventCaller<std::string, int, Event, bool&> eventCaller;

    // Add an event to the event instance table.
    // Specify the event to be called by setting key.
    std::string eventKey = "Key1";
    eventCaller.instTable_->Add(eventKey, std::make_unique<Event>());

    // Add a function to the function table.
    // Specify the function to be called by setting key.
    int funcKeyOnCall = 0;
    eventCaller.funcTable_->Add(funcKeyOnCall, &Event::OnCall);

    // Call the event.
    bool isCalled = false;
    eventCaller.Call(eventKey, funcKeyOnCall, isCalled);

    EXPECT_TRUE(isCalled);
}