#pragma once

#if defined(__MINGW32__) && !defined(_MSC_VER)
#include <windows.h>
#else
#include <Windows.h>
#endif
#include <string>

namespace ChameleonCompat
{
    std::string GetConfigDir();
    std::string GetSettingsPath();
    std::string GetLogPath();
    bool EnsureDirectory(const std::string& path);
    void InitLog(HMODULE module);
    void CloseLog();
    void Log(const char* fmt, ...);
    void LogLastError(const char* what);
    void LogHResult(const char* what, HRESULT hr);
    bool IsWine();
    std::string GetModulePath(HMODULE module);
}
