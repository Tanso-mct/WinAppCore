﻿#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely used portions from Windows headers
#include <windows.h>

#include <vector>
#include <memory>
#include <unordered_map>

namespace WACore
{

template <typename FUNC_KEY, typename EVENT, typename... Args>
class IEventFuncTable
{
public:
    virtual ~IEventFuncTable() = default;

    virtual bool Has(FUNC_KEY funcKey) = 0;
    virtual void(EVENT::*Get(FUNC_KEY funcKey))(Args...) = 0;
    
    virtual HRESULT Add(FUNC_KEY funcKey, void (EVENT::*func)(Args...)) = 0;
    virtual HRESULT Del(FUNC_KEY funcKey) = 0;

    virtual int GetSize() = 0;
    virtual void Clear() = 0;
};

template <typename EVENT_KEY, typename EVENT>
class IEventInstTable
{
public:
    virtual ~IEventInstTable() = default;

    virtual bool Has(EVENT_KEY key) = 0;
    virtual std::unique_ptr<EVENT>& Get(EVENT_KEY key, int id) = 0;

    virtual int Add(EVENT_KEY key, std::unique_ptr<EVENT> event) = 0;
    virtual HRESULT Del(EVENT_KEY key, int id) = 0;

    virtual std::unique_ptr<EVENT> Take(EVENT_KEY key, int id) = 0;
    virtual HRESULT Put(EVENT_KEY key, int id, std::unique_ptr<EVENT> event) = 0;

    virtual int GetSize() = 0;
    virtual void Clear() = 0;
};

template <typename EVENT_KEY, typename FUNC_KEY, typename... Args>
class IEventCaller
{
public:
    virtual ~IEventCaller() = default;
    virtual void Call(EVENT_KEY eventKey, FUNC_KEY funcKey, Args... args) = 0;
};

template <typename FUNC_KEY, typename EVENT, typename... Args>
class EventFuncTable : public IEventFuncTable<FUNC_KEY, EVENT, Args...>
{
private:
    std::unordered_map<FUNC_KEY, void (EVENT::*)(Args...)> funcs_;

public:
    EventFuncTable() = default;
    virtual ~EventFuncTable() override = default;

    // Delete copy constructor and operator=.
    EventFuncTable(const EventFuncTable&) = delete;
    EventFuncTable& operator=(const EventFuncTable&) = delete;

    virtual bool Has(FUNC_KEY funcKey) override
    {
        return funcs_.find(funcKey) != funcs_.end();
    }

    virtual void(EVENT::*Get(FUNC_KEY funcKey))(Args...) override
    {
        if (funcs_.find(funcKey) == funcs_.end()) return nullptr; // Key not found.
        return funcs_[funcKey];
    }

    virtual HRESULT Add(FUNC_KEY funcKey, void (EVENT::*func)(Args...)) override
    {
        if (funcs_.find(funcKey) != funcs_.end()) return E_FAIL; // Key already exists.

        funcs_[funcKey] = func;
        return S_OK;
    }

    virtual HRESULT Del(FUNC_KEY funcKey) override
    {
        if (funcs_.find(funcKey) == funcs_.end()) return E_FAIL; // Key not found.

        funcs_.erase(funcKey);
        return S_OK;
    }

    virtual int GetSize() override
    {
        return funcs_.size();
    }

    virtual void Clear() override
    {
        funcs_.clear();
    }
};

template <typename EVENT_KEY, typename EVENT>
class EventInstTable : public IEventInstTable<EVENT_KEY, EVENT>
{
private:
    std::unordered_map<EVENT_KEY, std::vector<std::unique_ptr<EVENT>>> events_;
    std::unique_ptr<EVENT> emptyEvent_ = nullptr;

public:
    EventInstTable() = default;
    virtual ~EventInstTable() override = default;

    // Delete copy constructor and operator=.
    EventInstTable(const EventInstTable&) = delete;
    EventInstTable& operator=(const EventInstTable&) = delete;

    virtual bool Has(EVENT_KEY eventKey) override
    {
        return events_.find(eventKey) != events_.end();
    }

    virtual std::unique_ptr<EVENT>& Get(EVENT_KEY eventKey, int id) override
    {
        if (events_.find(eventKey) == events_.end()) return emptyEvent_; // Key not found.
        if (id < 0 || id >= events_[eventKey].size()) return emptyEvent_; // Invalid id.

        return events_[eventKey][id];
    }

    virtual int Add(EVENT_KEY eventKey, std::unique_ptr<EVENT> event) override
    {   
        events_[eventKey].emplace_back(std::move(event));
        return events_[eventKey].size() - 1;
    }

    virtual HRESULT Del(EVENT_KEY eventKey, int id) override
    {
        if (events_.find(eventKey) == events_.end()) return E_FAIL; // Key not found.
        if (id < 0 || id >= events_[eventKey].size()) return E_FAIL; // Invalid id.

        events_[eventKey][id].reset();
        return S_OK;
    }

    virtual std::unique_ptr<EVENT> Take(EVENT_KEY eventKey, int id) override
    {
        if (events_.find(eventKey) == events_.end()) return nullptr; // Key not found.
        if (id < 0 || id >= events_[eventKey].size()) return nullptr; // Invalid id.

        std::unique_ptr<EVENT> event = std::move(events_[eventKey][id]);
        events_[eventKey][id] = nullptr;
        return event;
    }

    virtual HRESULT Put(EVENT_KEY eventKey, int id, std::unique_ptr<EVENT> event) override
    {
        if (events_.find(eventKey) == events_.end()) return E_FAIL; // Key not found.
        if (id < 0 || id >= events_[eventKey].size()) return E_FAIL; // Invalid id.

        events_[eventKey][id] = std::move(event);
        return S_OK;
    }

    virtual int GetSize() override
    {
        return events_.size();
    }

    virtual void Clear() override
    {
        events_.clear();
    }
};

template <typename EVENT_KEY, typename FUNC_KEY, typename EVENT, typename... Args>
class EventCaller : public IEventCaller<EVENT_KEY, FUNC_KEY, Args...>
{
public:
    std::unique_ptr<IEventFuncTable<FUNC_KEY, EVENT, Args...>> funcTable_ = nullptr;
    std::unique_ptr<IEventInstTable<EVENT_KEY, EVENT>> instTable_ = nullptr;

    EventCaller()
    {
        funcTable_ = std::make_unique<EventFuncTable<FUNC_KEY, EVENT, Args...>>();
        instTable_ = std::make_unique<EventInstTable<EVENT_KEY, EVENT>>();
    }

    EventCaller
    (
        std::unique_ptr<IEventFuncTable<FUNC_KEY, EVENT, Args...>> funcTable, 
        std::unique_ptr<IEventInstTable<EVENT_KEY, EVENT>> instTable
    ) : funcTable_(std::move(funcTable)), instTable_(std::move(instTable)) {}

    virtual ~EventCaller() override = default;

    virtual void Call(EVENT_KEY eventKey, FUNC_KEY funcKey, Args... args) override
    {
        if (!funcTable_) return; // Function table not found.
        if (!instTable_) return; // Instance table not found.

        if (!funcTable_->Has(funcKey)) return; // Function key not found.
        if (!instTable_->Has(eventKey)) return; // Event key not found.

        for (int i = 0; i < instTable_->GetSize(); i++) // Call all events with the key.
        {
            if (instTable_->Get(eventKey, i) == nullptr) continue; // Skip null events.
            (instTable_->Get(eventKey, i).get()->*(funcTable_->Get(funcKey)))(args...);
        }
    }
};

} // namespace WACore