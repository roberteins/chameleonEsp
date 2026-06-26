#include "includes.hpp"
#include "Compatibility.hpp"

#include <cstdarg>
#include <cstdio>
#include <ctime>

namespace ChameleonCompat
{
    static FILE* g_LogFile = nullptr;
    static std::mutex g_LogMutex;

    static std::string GetEnv(const char* name)
    {
        char value[MAX_PATH * 4]{};
        const DWORD len = GetEnvironmentVariableA(name, value, static_cast<DWORD>(sizeof(value)));
        if (len == 0 || len >= sizeof(value))
            return {};
        return value;
    }

    static std::string TrimTrailingSlash(std::string path)
    {
        while (!path.empty() && (path.back() == '\\' || path.back() == '/'))
            path.pop_back();
        return path;
    }

    bool EnsureDirectory(const std::string& path)
    {
        if (path.empty())
            return false;

        std::string current;
        for (size_t i = 0; i < path.size(); ++i)
        {
            const char c = path[i];
            current.push_back(c);
            if (c != '\\' && c != '/')
                continue;
            if (current.size() <= 3) // drive root, e.g. C:\
                continue;
            CreateDirectoryA(current.c_str(), nullptr);
        }

        if (CreateDirectoryA(path.c_str(), nullptr) || GetLastError() == ERROR_ALREADY_EXISTS)
            return true;
        return false;
    }

    std::string GetConfigDir()
    {
        std::string overrideDir = TrimTrailingSlash(GetEnv("CHAMELEONESP_CONFIG_DIR"));
        if (!overrideDir.empty())
            return overrideDir;

        std::string appData = TrimTrailingSlash(GetEnv("APPDATA"));
        if (!appData.empty())
            return appData + "\\chameleonEsp";

        std::string userProfile = TrimTrailingSlash(GetEnv("USERPROFILE"));
        if (!userProfile.empty())
            return userProfile + "\\chameleonEsp";

        std::string home = TrimTrailingSlash(GetEnv("HOME"));
        if (!home.empty())
            return home + "\\.config\\chameleonEsp";

        return "C:\\chameleonEsp";
    }

    std::string GetSettingsPath()
    {
        return GetConfigDir() + "\\settings.ini";
    }

    std::string GetLogPath()
    {
        const std::string overridePath = GetEnv("CHAMELEONESP_LOG_FILE");
        if (!overridePath.empty())
            return overridePath;
        return GetConfigDir() + "\\chameleonEsp.log";
    }

    std::string GetModulePath(HMODULE module)
    {
        char path[MAX_PATH * 4]{};
        const DWORD len = GetModuleFileNameA(module, path, static_cast<DWORD>(sizeof(path)));
        if (len == 0 || len >= sizeof(path))
            return {};
        return path;
    }

    bool IsWine()
    {
        HMODULE ntdll = GetModuleHandleA("ntdll.dll");
        if (!ntdll)
            return false;
        return GetProcAddress(ntdll, "wine_get_version") != nullptr;
    }

    void InitLog(HMODULE module)
    {
        std::lock_guard<std::mutex> lock(g_LogMutex);
        EnsureDirectory(GetConfigDir());
        if (!g_LogFile)
            g_LogFile = std::fopen(GetLogPath().c_str(), "ab");
        if (!g_LogFile)
            return;

        std::time_t now = std::time(nullptr);
        std::fprintf(g_LogFile, "\n=== chameleonEsp start %lld ===\n", static_cast<long long>(now));
        std::fprintf(g_LogFile, "pid=%lu module=%p module_path=%s wine=%s config_dir=%s\n",
            GetCurrentProcessId(), module, GetModulePath(module).c_str(), IsWine() ? "true" : "false", GetConfigDir().c_str());
        std::fflush(g_LogFile);
    }

    void CloseLog()
    {
        std::lock_guard<std::mutex> lock(g_LogMutex);
        if (g_LogFile)
        {
            std::fprintf(g_LogFile, "=== chameleonEsp stop ===\n");
            std::fclose(g_LogFile);
            g_LogFile = nullptr;
        }
    }

    void Log(const char* fmt, ...)
    {
        std::lock_guard<std::mutex> lock(g_LogMutex);
        if (!g_LogFile)
            return;
        std::time_t now = std::time(nullptr);
        std::fprintf(g_LogFile, "[%lld] ", static_cast<long long>(now));
        va_list args;
        va_start(args, fmt);
        std::vfprintf(g_LogFile, fmt, args);
        va_end(args);
        std::fprintf(g_LogFile, "\n");
        std::fflush(g_LogFile);
    }

    void LogLastError(const char* what)
    {
        Log("%s failed GetLastError=%lu", what, GetLastError());
    }

    void LogHResult(const char* what, HRESULT hr)
    {
        Log("%s failed HRESULT=0x%08lx", what, static_cast<unsigned long>(hr));
    }
}
