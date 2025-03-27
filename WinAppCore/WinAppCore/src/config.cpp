#include "pch.h"
#include "config.h"

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