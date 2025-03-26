#pragma once
#include "config.h"

#include <iostream>
#include <initializer_list>
#include <string>
#include <string_view>

namespace WACore
{

WIN_APP_CORE unsigned int& ErrorCount();
WIN_APP_CORE unsigned int& WarningCount();

class WIN_APP_CORE IssueNotifier
{
public:
    IssueNotifier();
    IssueNotifier(unsigned int errorCount, unsigned int warningCount);
    virtual ~IssueNotifier();
};

WIN_APP_CORE void Cout(std::initializer_list<std::string_view> args);
WIN_APP_CORE void CoutErr(std::initializer_list<std::string_view> args);
WIN_APP_CORE void CoutWrn(std::initializer_list<std::string_view> args);
WIN_APP_CORE void CoutInfo(std::initializer_list<std::string_view> args);
WIN_APP_CORE void CoutDebug(std::initializer_list<std::string_view> args);

} // namespace WACore