#include "WinAppCore_Test/pch.h"
#include "WinAppCore/include/WACore.h"
#pragma comment(lib, "WinAppCore.lib")

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

TEST(WinAppCore_container, Add) 
{
    std::unique_ptr<Item> item1 = std::make_unique<Item>("Item1");
    std::unique_ptr<Item> item2 = std::make_unique<Item>("Item2");

    std::unique_ptr<WACore::IContainer> container = std::make_unique<WACore::Container>();
    int idItem1 = container->Add(std::move(item1));
    int idItem2 = container->Add(std::move(item2));

    EXPECT_EQ(0, idItem1);
    EXPECT_EQ(1, idItem2);
}

TEST(WinAppCore_container, Delete) 
{
    std::unique_ptr<Item> item1 = std::make_unique<Item>("Item1");
    std::unique_ptr<Item> item2 = std::make_unique<Item>("Item2");

    std::unique_ptr<WACore::IContainer> container = std::make_unique<WACore::Container>();
    int idItem1 = container->Add(std::move(item1));
    int idItem2 = container->Add(std::move(item2));

    HRESULT hr = container->Del(idItem1);
    EXPECT_EQ(S_OK, hr);

    hr = container->Del(idItem2);
    EXPECT_EQ(S_OK, hr);
}

TEST(WinAppCore_container, Get) 
{
    int idItem1 = 0;
    int idItem2 = 1;
    std::unique_ptr<WACore::IContainer> container = nullptr;
    {
        std::unique_ptr<Item> item1 = std::make_unique<Item>("Item1");
        std::unique_ptr<Item> item2 = std::make_unique<Item>("Item2");

        container = std::make_unique<WACore::Container>();
        idItem1 = container->Add(std::move(item1));
        idItem2 = container->Add(std::move(item2));
    }

    {
        std::unique_ptr<WACore::IContainable>& item = container->Get(idItem1);
        EXPECT_NE(nullptr, item);
    }

    {
        std::unique_ptr<WACore::IContainable>& item = container->Get(idItem2);
        EXPECT_NE(nullptr, item);
    }
}

TEST(WinAppCore_container, Take) 
{
    int idItem1 = 0;
    int idItem2 = 1;
    std::unique_ptr<WACore::IContainer> container = nullptr;
    {
        std::unique_ptr<Item> item1 = std::make_unique<Item>("Item1");
        std::unique_ptr<Item> item2 = std::make_unique<Item>("Item2");

        container = std::make_unique<WACore::Container>();
        idItem1 = container->Add(std::move(item1));
        idItem2 = container->Add(std::move(item2));
    }

    {
        std::unique_ptr<WACore::IContainable> item = container->Take(idItem1);
        EXPECT_NE(nullptr, item);
    }

    {
        std::unique_ptr<WACore::IContainable> item = container->Take(idItem2);
        EXPECT_NE(nullptr, item);
    }
}

TEST(WinAppCore_container, Put) 
{
    int idItem1 = 0;
    int idItem2 = 1;
    std::unique_ptr<WACore::IContainer> container = nullptr;
    {
        std::unique_ptr<Item> item1 = std::make_unique<Item>("Item1");
        std::unique_ptr<Item> item2 = std::make_unique<Item>("Item2");

        container = std::make_unique<WACore::Container>();
        idItem1 = container->Add(std::move(item1));
        idItem2 = container->Add(std::move(item2));
    }

    {
        std::unique_ptr<Item> changedItem1 = std::make_unique<Item>("ChangedItem1");
        HRESULT hr = container->Put(idItem1, std::move(changedItem1));
        EXPECT_EQ(S_OK, hr);
    }

    {
        std::unique_ptr<Item> changedItem2 = std::make_unique<Item>("ChangedItem2");
        HRESULT hr = container->Put(idItem2, std::move(changedItem2));
        EXPECT_EQ(S_OK, hr);
    }
}

TEST(WinAppCore_container, Retrieval) 
{
    int idItem1 = 0;
    int idItem2 = 1;
    std::unique_ptr<WACore::IContainer> container = nullptr;
    {
        std::unique_ptr<Item> item1 = std::make_unique<Item>("Item1");
        std::unique_ptr<Item> item2 = std::make_unique<Item>("Item2");

        container = std::make_unique<WACore::Container>();
        idItem1 = container->Add(std::move(item1));
        idItem2 = container->Add(std::move(item2));
    }

    {
        std::unique_ptr<WACore::IContainable> item = container->Take(idItem1);
        EXPECT_NE(nullptr, item);

        HRESULT hr = container->Put(idItem1, std::move(item));
        EXPECT_EQ(S_OK, hr);
    }

    {
        std::unique_ptr<WACore::IContainable> item = container->Take(idItem2);
        EXPECT_NE(nullptr, item);

        HRESULT hr = container->Put(idItem2, std::move(item));
        EXPECT_EQ(S_OK, hr);
    }
}

TEST(WinAppCore_container, Size)
{
    std::unique_ptr<WACore::IContainer> container = std::make_unique<WACore::Container>();
    EXPECT_EQ(0, container->GetSize());

    std::unique_ptr<Item> item1 = std::make_unique<Item>("Item1");
    std::unique_ptr<Item> item2 = std::make_unique<Item>("Item2");

    container->Add(std::move(item1));
    EXPECT_EQ(1, container->GetSize());

    container->Add(std::move(item2));
    EXPECT_EQ(2, container->GetSize());
}

TEST(WinAppCore_container, Clear)
{
    std::unique_ptr<WACore::IContainer> container = std::make_unique<WACore::Container>();
    EXPECT_EQ(0, container->GetSize());

    std::unique_ptr<Item> item1 = std::make_unique<Item>("Item1");
    std::unique_ptr<Item> item2 = std::make_unique<Item>("Item2");

    container->Add(std::move(item1));
    container->Add(std::move(item2));

    container->Clear();
    EXPECT_EQ(0, container->GetSize());
}