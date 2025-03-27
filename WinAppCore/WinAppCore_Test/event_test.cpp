#include "pch.h"

#include "WACore.h"

class Item : public WACore::IContainable
{
private:
    std::string name_;

public:
    Item(std::string_view name) : name_(name)
    {
        std::cout << name_ << " is created." << std::endl;
    }

    virtual ~Item() override
    {
        std::cout << name_ << " is destroyed." << std::endl;
    }
};

class Event
{
public:
    Event() = default;
    virtual ~Event() = default;

    // In functions within events called by EventCaller, the container is always passed as the first argument.
    // The container passed is the container corresponding to the event key.
    void OnCall(std::unique_ptr<WACore::IContainer>& container, bool& isCalled)
    {
        std::cout << "Event is called" << std::endl;
        std::cout << "Container size : " << container->GetSize() << std::endl;
        isCalled = true;
    }
};

TEST(WinAppCore_event, CallEvent)
{
    // EVENT_KEY -> std::string
    // FUNC_KEY -> int
    // EVENT -> Event
    // Args -> bool&
    WACore::EventSet<std::string, int, Event, bool&> eventSet;

    // Register the function to be called in the event instance.
    // Specify the function to be called by setting key.
    int funcKeyOnCall = 0;
    eventSet.AddFunc(0, &Event::OnCall);

    // Add a event key to the event caller.
    std::string eventKey = "Key1";
    eventSet.Caller()->AddKey(eventKey);

    // Add an event to the event caller.
    // Specify the event to be called by setting key.
    eventSet.Caller()->AddEvent(eventKey, std::make_unique<Event>());

    // Create a container and add items to it.
    std::unique_ptr<WACore::IContainer> container = std::make_unique<WACore::Container>();
    container->Add(std::make_unique<Item>("Item1"));
    container->Add(std::make_unique<Item>("Item2"));

    // Add the container to the event caller.
    // In the function of an event, the container is always passed as an argument.
    // The container passed is the container corresponding to the event key.
    eventSet.Caller()->AddContainer(eventKey, std::move(container));

    // Call the event.
    bool isCalled = false;
    eventSet.Call(eventKey, funcKeyOnCall, isCalled);

    EXPECT_TRUE(isCalled);
}