#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <initializer_list>

namespace WACore
{

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

} // namespace WACore