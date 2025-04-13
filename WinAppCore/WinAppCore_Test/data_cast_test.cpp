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

    std::string GetName() const
    {
        return name_;
    }
};

TEST(WinAppCore_data_cast, PointerCast)
{
    WACore::IContainable* item = new Item("Item");
    Item* castedItem = WACore::As<Item>(item);

    std::string name = castedItem->GetName();
    EXPECT_EQ(name, "Item");

    delete item;
}

TEST(WinAppCore_data_cast, UniquePointerCast)
{
    std::unique_ptr<WACore::IContainable> item = std::make_unique<Item>("Item");
    std::unique_ptr<Item> castedItem = WACore::UniqueAs<Item>(item);

    std::string name = castedItem->GetName();
    EXPECT_EQ(name, "Item");
}

TEST(WinAppCore_data_cast, RevertCast)
{
    std::string name;
    std::unique_ptr<WACore::IContainable> item = std::make_unique<Item>("Item");
    {
        WACore::RevertCast<Item, WACore::IContainable> revertCast(item);

        std::unique_ptr<Item>& castedItem = revertCast();
        name = castedItem->GetName();
    }

    ASSERT_NE(item, nullptr);
    EXPECT_EQ(name, "Item");
}