#pragma once

// DLL export and import settings.
#define BUILDING_WACore_DLL
#ifdef BUILDING_WACore_DLL
#define WIN_APP_CORE __declspec(dllexport)
#else
#define WIN_APP_CORE __declspec(dllimport)
#endif

#include <string>

namespace WACore
{

class WIN_APP_CORE CoutConfig
{
public:
    static std::string& START_TAG();
    static std::string& END_TAG();

    static std::string& COLOR_NORMAL();
    static std::string& COLOR_ERROR();
    static std::string& COLOR_WARNING();
    static std::string& COLOR_INFO();
    static std::string& COLOR_DEBUG();

    static std::string& COLOR_RESET();
};

} // namespace WACore