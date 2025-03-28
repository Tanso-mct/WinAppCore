#include "pch.h"
#include "console_output.h"

std::string &WACore::CoutConfig::START_TAG()
{
    static std::string startTag = "[----------]";
    return startTag;
}

std::string &WACore::CoutConfig::END_TAG()
{
    static std::string endTag = "[      END ]";
    return endTag;
}

std::string &WACore::CoutConfig::COLOR_NORMAL()
{
    static std::string colorNormal = "\033[1;32m";
    return colorNormal;
}

std::string &WACore::CoutConfig::COLOR_ERROR()
{
    static std::string colorError = "\033[1;31m";
    return colorError;
}

std::string &WACore::CoutConfig::COLOR_WARNING()
{
    static std::string colorWarning = "\033[1;33m";
    return colorWarning;
}

std::string &WACore::CoutConfig::COLOR_INFO()
{
    static std::string colorInfo = "\033[1;34m";
    return colorInfo;
}

std::string &WACore::CoutConfig::COLOR_DEBUG()
{
    static std::string colorDebug = "\033[1;36m";
    return colorDebug;
}

std::string &WACore::CoutConfig::COLOR_RESET()
{
    static std::string colorReset = "\033[0m";
    return colorReset;
}

WIN_APP_CORE unsigned int &WACore::ErrorCount()
{
    static unsigned int errorCount = 0;
    return errorCount;
}

WIN_APP_CORE unsigned int &WACore::WarningCount()
{
    static unsigned int warningCount = 0;
    return warningCount;
}

WACore::IssueNotifier::IssueNotifier()
{
    ErrorCount() = 0;
    WarningCount() = 0;
}

WACore::IssueNotifier::IssueNotifier(unsigned int errorCount, unsigned int warningCount)
{
    ErrorCount() = errorCount;
    WarningCount() = warningCount;
}

WACore::IssueNotifier::~IssueNotifier()
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

WIN_APP_CORE void WACore::Cout(std::initializer_list<std::string_view> args)
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

WIN_APP_CORE void WACore::CoutErr(std::initializer_list<std::string_view> args)
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

WIN_APP_CORE void WACore::CoutWrn(std::initializer_list<std::string_view> args)
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

WIN_APP_CORE void WACore::CoutInfo(std::initializer_list<std::string_view> args)
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

WIN_APP_CORE void WACore::CoutDebug(std::initializer_list<std::string_view> args)
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
