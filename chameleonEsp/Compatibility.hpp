#pragma once

#include <Windows.h>
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
