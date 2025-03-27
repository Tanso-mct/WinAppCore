#pragma once

#include <string>
#include <memory>
#include <string_view>
#include <iostream>
#include <vector>
#include <initializer_list>
#include <unordered_map>

#define WIN32_LEAN_AND_MEAN // Exclude rarely used portions from Windows headers
#include <windows.h>

namespace WACore
{

constexpr int ID_INVALID = -1;

class CoutConfig
{
public:
    static std::string& START_TAG()
    {
        static std::string startTag = "[----------]";
        return startTag;
    }
    static std::string& END_TAG()
    {
        static std::string endTag = "[      END ]";
        return endTag;
    }

    static std::string& COLOR_NORMAL()
    {
        static std::string colorNormal = "\033[1;32m";
        return colorNormal;
    }
    static std::string& COLOR_ERROR()
    {
        static std::string colorError = "\033[1;31m";
        return colorError;
    }
    static std::string& COLOR_WARNING()
    {
        static std::string colorWarning = "\033[1;33m";
        return colorWarning;
    }
    static std::string& COLOR_INFO()
    {
        static std::string colorInfo = "\033[1;34m";
        return colorInfo;
    }
    static std::string& COLOR_DEBUG()
    {
        static std::string colorDebug = "\033[1;36m";
        return colorDebug;
    }

    static std::string& COLOR_RESET()
    {
        static std::string colorReset = "\033[0m";
        return colorReset;
    }
};

template <typename T, typename S>
T* As(S* source)
{
    T* target = dynamic_cast<T*>(source);
    return target;
}

template <typename T, typename S>
std::unique_ptr<T> UniqueAs(std::unique_ptr<S>& source)
{
    T* target = dynamic_cast<T*>(source.get());

    if (target) return std::unique_ptr<T>(static_cast<T*>(source.release()));
    else return nullptr;
}

/**
 * Casts an instance of type unique_ptr to the specified type and 
 * returns ownership to the cast source when it exits scope.
 * Casting from one type of RevertCast to another is discouraged.
 */
template <typename D, typename S>
class RevertCast
{
private:
    std::unique_ptr<S>& src_;
    std::unique_ptr<D> casted_ = nullptr;

public:
    RevertCast(std::unique_ptr<S>& src) : src_(src)
    {
        D* dstPtr = dynamic_cast<D*>(src.get());
        if (dstPtr) casted_.reset(dynamic_cast<D*>(src.release()));
    }

    ~RevertCast()
    {
        src_.reset(dynamic_cast<S*>(casted_.release()));
    }

    std::unique_ptr<D>& operator()() 
    {
        return casted_;
    }
};

inline unsigned int& ErrorCount()
{
    static unsigned int errorCount = 0;
    return errorCount;
}
inline unsigned int& WarningCount()
{
    static unsigned int warningCount = 0;
    return warningCount;
}

class IssueNotifier
{
public:
    IssueNotifier()
    {
        ErrorCount() = 0;
        WarningCount() = 0;
    }
    IssueNotifier(unsigned int errorCount, unsigned int warningCount)
    {
        ErrorCount() = errorCount;
        WarningCount() = warningCount;
    }

    virtual ~IssueNotifier()
    {
        std::string color = "\033[1;32m";

        std::cout << std::endl;
    
        std::cout << color << "[==========]" << WACore::CoutConfig::COLOR_RESET();
        std::cout << " Executing results." << std::endl;
    
        if (WACore::ErrorCount() == 0 && WACore::WarningCount() == 0)
        {
            std::cout << color << "[==========]" << WACore::CoutConfig::COLOR_RESET();
            std::cout << " No issues found." << std::endl;
            return;
        }
    
        if (WACore::ErrorCount() > 0)
        {
            std::cout << color << "[==========]" << WACore::CoutConfig::COLOR_RESET();
            std::cout << " " << ErrorCount() << " error(s) found." << std::endl;
        }
    
        if (WACore::WarningCount() > 0)
        {
            std::cout << color << "[==========]" << WACore::CoutConfig::COLOR_RESET();
            std::cout << " " << WarningCount() << " warning(s) found." << std::endl;
        }
    
        std::cout << std::endl;
    }
};

inline void Cout(std::initializer_list<std::string_view> args)
{
    std::cout << WACore::CoutConfig::COLOR_NORMAL() 
    << WACore::CoutConfig::START_TAG() 
    << WACore::CoutConfig::COLOR_RESET() << " ";

    for (const auto& arg : args) std::cout << arg << std::endl;

    if (args.size() != 1)
    {
        std::cout << WACore::CoutConfig::COLOR_NORMAL() 
        << WACore::CoutConfig::END_TAG() 
        << WACore::CoutConfig::COLOR_RESET() << std::endl;
    }
}

inline void CoutErr(std::initializer_list<std::string_view> args)
{
    std::cout << WACore::CoutConfig::COLOR_ERROR() 
    << WACore::CoutConfig::START_TAG() 
    << WACore::CoutConfig::COLOR_RESET() << " ";

    for (const auto& arg : args) std::cout << arg << std::endl;
    
    if (args.size() != 1)
    {
        std::cout << WACore::CoutConfig::COLOR_ERROR() 
        << WACore::CoutConfig::END_TAG() 
        << WACore::CoutConfig::COLOR_RESET() << std::endl;
    }

    ErrorCount()++;
}

inline void CoutWrn(std::initializer_list<std::string_view> args)
{
    std::cout << WACore::CoutConfig::COLOR_WARNING() 
    << WACore::CoutConfig::START_TAG() 
    << WACore::CoutConfig::COLOR_RESET() << " ";

    for (const auto& arg : args) std::cout << arg << std::endl;
    
    if (args.size() != 1)
    {
        std::cout << WACore::CoutConfig::COLOR_WARNING() 
        << WACore::CoutConfig::END_TAG() 
        << WACore::CoutConfig::COLOR_RESET() << std::endl;
    }

    WarningCount()++;
}

inline void CoutInfo(std::initializer_list<std::string_view> args)
{
    std::cout << WACore::CoutConfig::COLOR_INFO() 
    << WACore::CoutConfig::START_TAG() 
    << WACore::CoutConfig::COLOR_RESET() << " ";

    for (const auto& arg : args) std::cout << arg << std::endl;
    
    if (args.size() != 1)
    {
        std::cout << WACore::CoutConfig::COLOR_INFO() 
        << WACore::CoutConfig::END_TAG() 
        << WACore::CoutConfig::COLOR_RESET() << std::endl;
    }
}

inline void CoutDebug(std::initializer_list<std::string_view> args)
{
    std::cout << WACore::CoutConfig::COLOR_DEBUG() 
    << WACore::CoutConfig::START_TAG() 
    << WACore::CoutConfig::COLOR_RESET() << " ";

    for (const auto& arg : args) std::cout << arg << std::endl;
    
    if (args.size() != 1)
    {
        std::cout << WACore::CoutConfig::COLOR_DEBUG() 
        << WACore::CoutConfig::END_TAG() 
        << WACore::CoutConfig::COLOR_RESET() << std::endl;
    }
}

class IContainable
{
public:
    virtual ~IContainable() = default;
};

class IContainer : public IContainable
{
public:
    virtual ~IContainer() = default;

    virtual int Add(std::unique_ptr<IContainable> data) = 0;
    virtual HRESULT Del(int id) = 0;

    virtual std::unique_ptr<IContainable>& Get(int id) = 0;

    virtual std::unique_ptr<IContainable> Take(int id) = 0;
    virtual HRESULT Put(int id, std::unique_ptr<IContainable> data) = 0;

    virtual int GetSize() = 0;
    virtual void Clear() = 0;
};

class Container : public IContainer
{
private:
    std::vector<std::unique_ptr<IContainable>> datas_;
    std::unique_ptr<IContainable> emptyData_ = nullptr;

public:
    Container() = default;
    virtual ~Container() override = default;

    // Delete copy constructor and operator=.
    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;

    int Add(std::unique_ptr<IContainable> data);
    HRESULT Del(int id) override
    {
        if (id < 0 || id >= datas_.size()) return E_FAIL;

        datas_[id].reset();
        return S_OK;
    }

    std::unique_ptr<IContainable>& Get(int id) override
    {
        if (id < 0 || id >= datas_.size()) return emptyData_;
        return datas_[id];
    }

    std::unique_ptr<IContainable> Take(int id) override
    {
        if (id < 0 || id >= datas_.size()) return nullptr;
        return std::move(datas_[id]);
    }
    HRESULT Put(int id, std::unique_ptr<IContainable> data) override
    {
        if (id < 0 || id >= datas_.size()) return E_FAIL;

        datas_[id] = std::move(data);
        return S_OK;
    }
    
    int GetSize() override
    {
        return datas_.size();
    }
    void Clear() override
    {
        datas_.clear();
    }
};

template <typename EVENT_KEY, typename EVENT, typename... Args>
class EventCaller
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
class EventSet
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

class IWPEvent
{
public:
    virtual ~IWPEvent() = default;

    virtual void OnSetFocus(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnKillFocus(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnSize(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnUpdate(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnMove(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnDestroy(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnKeyDown(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnKeyUp(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnMouse(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
};

using WPEventSet = EventSet<HWND, UINT, IWPEvent, UINT, WPARAM, LPARAM>;
inline void RegisterWPEventFuncs(WPEventSet& eventSet)
{
    eventSet.AddFunc(WM_SETFOCUS, &IWPEvent::OnSetFocus);
    eventSet.AddFunc(WM_KILLFOCUS, &IWPEvent::OnKillFocus);
    eventSet.AddFunc(WM_PAINT, &IWPEvent::OnUpdate);
    eventSet.AddFunc(WM_SIZE, &IWPEvent::OnSize);
    eventSet.AddFunc(WM_PAINT, &IWPEvent::OnUpdate);
    eventSet.AddFunc(WM_MOVE, &IWPEvent::OnMove);
    eventSet.AddFunc(WM_DESTROY, &IWPEvent::OnDestroy);

    eventSet.AddFunc(WM_KEYDOWN, &IWPEvent::OnKeyDown);
    eventSet.AddFunc(WM_SYSKEYDOWN, &IWPEvent::OnKeyDown);
    eventSet.AddFunc(WM_KEYUP, &IWPEvent::OnKeyUp);
    eventSet.AddFunc(WM_SYSKEYUP, &IWPEvent::OnKeyUp);

    eventSet.AddFunc(WM_LBUTTONDOWN, &IWPEvent::OnMouse);
    eventSet.AddFunc(WM_LBUTTONUP, &IWPEvent::OnMouse);
    eventSet.AddFunc(WM_RBUTTONDOWN, &IWPEvent::OnMouse);
    eventSet.AddFunc(WM_RBUTTONUP, &IWPEvent::OnMouse);
    eventSet.AddFunc(WM_MBUTTONDOWN, &IWPEvent::OnMouse);
    eventSet.AddFunc(WM_MBUTTONUP, &IWPEvent::OnMouse);
    eventSet.AddFunc(WM_MOUSEWHEEL, &IWPEvent::OnMouse);
    eventSet.AddFunc(WM_MOUSEMOVE, &IWPEvent::OnMouse);
}

} // namespace WACore
