#include "pch.h"

#include "WACore.h"

TEST(WinAppCore_console, Cout)
{
    std::unique_ptr<WACore::IConsoleOuter> consoleOuter = std::make_unique<WACore::ConsoleOuter>();
    consoleOuter->Cout({"Normal Message. This is one line."});
    consoleOuter->Cout({"Normal Message", "This is multiple lines."});
    EXPECT_TRUE(true);
}

TEST(WinAppCore_console, CoutError)
{
    std::unique_ptr<WACore::IConsoleOuter> consoleOuter = std::make_unique<WACore::ConsoleOuter>();
    consoleOuter->CoutErr({"Error Message. This is one line."});
    consoleOuter->CoutErr({"Error Message", "This is multiple lines."});
    EXPECT_TRUE(true);
}

TEST(WinAppCore_console, CoutWarning)
{
    std::unique_ptr<WACore::IConsoleOuter> consoleOuter = std::make_unique<WACore::ConsoleOuter>();
    consoleOuter->CoutWrn({"Warning Message. This is one line."});
    consoleOuter->CoutWrn({"Warning Message", "This is multiple lines."});
    EXPECT_TRUE(true);
}

TEST(WinAppCore_console, CoutInfo)
{
    std::unique_ptr<WACore::IConsoleOuter> consoleOuter = std::make_unique<WACore::ConsoleOuter>();
    consoleOuter->CoutInfo({"Info Message. This is one line."});
    consoleOuter->CoutInfo({"Info Message", "This is multiple lines."});
    EXPECT_TRUE(true);
}

TEST(WinAppCore_console, CoutDebug)
{
    std::unique_ptr<WACore::IConsoleOuter> consoleOuter = std::make_unique<WACore::ConsoleOuter>();
    consoleOuter->CoutDebug({"Debug Message. This is one line."});
    consoleOuter->CoutDebug({"Debug Message", "This is multiple lines."});
    EXPECT_TRUE(true);
}

TEST(WinAppCore_console, EditSettting)
{
    std::unique_ptr<WACore::ConsoleOuter> consoleOuter = std::make_unique<WACore::ConsoleOuter>();
    consoleOuter->startTag_ = "[ TEST     ]";

    consoleOuter->Cout({"Normal Message."});
    consoleOuter->CoutErr({"Error Message."});
    consoleOuter->CoutWrn({"Warning Message."});
    consoleOuter->CoutInfo({"Info Message."});
    consoleOuter->CoutDebug({"Debug Message."});
}

TEST(WinAppCore_console, IssueNotification)
{
    std::unique_ptr<WACore::IConsoleOuter> consoleOuter = std::make_unique<WACore::ConsoleOuter>();

    std::unique_ptr<WACore::IIssueNotifier> issueNotifier = std::make_unique<WACore::IssueNotifier>
    (
        consoleOuter->GetErrorCount(), 
        consoleOuter->GetWarningCount()
    );

    consoleOuter->Cout({"Normal Message."});
    consoleOuter->CoutErr({"Error Message."});
    consoleOuter->CoutWrn({"Warning Message."});
    consoleOuter->CoutInfo({"Info Message."});
    consoleOuter->CoutDebug({"Debug Message."});
}