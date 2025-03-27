#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely used portions from Windows headers
#include <windows.h>

#include <vector>
#include <memory>

namespace WACore
{

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

    int Add(std::unique_ptr<IContainable> data)
    {
        datas_.emplace_back(std::move(data));
        return datas_.size() - 1;
    }
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

} // namespace WACore