#pragma once
#include "config.h"

#include "container.h"

#include <Windows.h>
#include <memory>
#include <unordered_map>

namespace WACore
{

template <typename EVENT_KEY, typename EVENT, typename... Args>
class WIN_APP_CORE EventCaller
{
private:
    std::unordered_map
    <
        EVENT_KEY, 
        std::pair<std::vector<std::unique_ptr<EVENT>>, std::unique_ptr<IContainer>>
    > eventTable_;

    inline std::vector<std::unique_ptr<EVENT>>& Events(EVENT_KEY key)
    {
        return eventTable_[key].first;
    }

    inline std::unique_ptr<EVENT>& Event(EVENT_KEY key, int id)
    {
        return eventTable_[key].first[id];
    }

    inline std::unique_ptr<IContainer>& Container(EVENT_KEY key)
    {
        return eventTable_[key].second;
    }

public:
    EventCaller() = default;
    virtual ~EventCaller() = default;

    // Delete copy constructor and operator=.
    EventCaller(const EventCaller&) = delete;
    EventCaller& operator=(const EventCaller&) = delete;

    HRESULT AddKey(EVENT_KEY key)
    {
        if (eventTable_.find(key) != eventTable_.end()) return E_FAIL;

        eventTable_[key] = std::make_pair(std::vector<std::unique_ptr<EVENT>>(), nullptr);
        return S_OK;
    }

    int AddEvent(EVENT_KEY key, std::unique_ptr<EVENT> event)
    {   
        if (eventTable_.find(key) == eventTable_.end()) return WACore::ID_INVALID;

        Events(key).emplace_back(std::move(event));
        return Events(key).size() - 1;
    }

    std::unique_ptr<EVENT> TakeEvent(EVENT_KEY key, int id)
    {
        if (eventTable_.find(key) == eventTable_.end()) return nullptr;
        if (id < 0 || id >= Events(key).size()) return nullptr;

        std::unique_ptr<EVENT> event = std::move(Event(key, id));
        return event;
    }

    HRESULT AddContainer(EVENT_KEY key, std::unique_ptr<IContainer> container)
    {
        if (eventTable_.find(key) == eventTable_.end()) return E_FAIL;

        Container(key) = std::move(container);
        return S_OK;
    }

    std::unique_ptr<IContainer> TakeContainer(EVENT_KEY key)
    {
        if (eventTable_.find(key) == eventTable_.end()) return nullptr;

        std::unique_ptr<IContainer> container = std::move(Container(key));
        return container;
    }

    void Call(EVENT_KEY key, void (EVENT::*func)(std::unique_ptr<IContainer>&, Args...), Args... args)
    {
        if (eventTable_.find(key) == eventTable_.end()) return;
        for (int i = 0; i < Events(key).size(); i++)
        {
            if (Event(key, i) == nullptr) continue;
            (Event(key, i).get()->*func)(Container(key), args...);
        }
    }
};

template <typename EVENT_KEY, typename FUNC_KEY, typename EVENT, typename... Args>
class WIN_APP_CORE EventSet
{
private:
    std::unordered_map<FUNC_KEY, void (EVENT::*)(std::unique_ptr<IContainer>&, Args...)> funcs_;
    std::unique_ptr<EventCaller<EVENT_KEY, EVENT, Args...>> caller_ = nullptr;

public:
    EventSet()
    {
        caller_ = std::make_unique<EventCaller<EVENT_KEY, EVENT, Args...>>();
    }
    virtual ~EventSet() = default;

    // Delete copy constructor and operator=.
    EventSet(const EventSet&) = delete;
    EventSet& operator=(const EventSet&) = delete;

    HRESULT AddFunc(FUNC_KEY key, void (EVENT::*func)(std::unique_ptr<IContainer>&, Args...))
    {
        if (funcs_.find(key) != funcs_.end()) return E_FAIL;

        funcs_[key] = func;
        return S_OK;
    }

    const std::unique_ptr<EventCaller<EVENT_KEY, EVENT, Args...>>& Caller() const { return caller_; }

    void Call(EVENT_KEY key, FUNC_KEY funcKey, Args... args)
    {
        if (funcs_.find(funcKey) == funcs_.end()) return;
        caller_->Call(key, funcs_[funcKey], args...);
    }
};

} // namespace WACore