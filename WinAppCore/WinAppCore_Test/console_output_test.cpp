#include "pch.h"

#include "WACore.h"

TEST(WinAppCore_console, EditConfig)
{
    WACore::CoutConfig::START_TAG() = "[ TEST     ]";
    EXPECT_TRUE(true);
}

TEST(WinAppCore_console, Cout)
{
    WACore::Cout({"Normal Message. This is one line."});
    WACore::Cout({"Normal Message", "This is multiple lines."});
    EXPECT_TRUE(true);
}

TEST(WinAppCore_console, CoutError)
{
    WACore::CoutErr({"Error Message. This is one line."});
    WACore::CoutErr({"Error Message", "This is multiple lines."});
    EXPECT_TRUE(true);
}

TEST(WinAppCore_console, CoutWarning)
{
    WACore::CoutWrn({"Warning Message. This is one line."});
    WACore::CoutWrn({"Warning Message", "This is multiple lines."});
    EXPECT_TRUE(true);
}

TEST(WinAppCore_console, CoutInfo)
{
    WACore::CoutInfo({"Info Message. This is one line."});
    WACore::CoutInfo({"Info Message", "This is multiple lines."});
    EXPECT_TRUE(true);
}

TEST(WinAppCore_console, CoutDebug)
{
    WACore::CoutDebug({"Debug Message. This is one line."});
    WACore::CoutDebug({"Debug Message", "This is multiple lines."});
    EXPECT_TRUE(true);
}

TEST(WinAppCore_console, IssueNotification)
{
    WACore::IssueNotifier issueNotifier;

    WACore::Cout({"Normal Message"});
    WACore::CoutErr({"Error Message"});
    WACore::CoutWrn({"Warning Message"});
}