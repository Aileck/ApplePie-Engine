#pragma once
#include "SDL.h"
#include "GL\glew.h"
#include "backends\imgui.h"
#include "backends\imgui_impl_sdl2.h"
#include "backends\imgui_impl_opengl3.h"
#include "Application.h"

#include <ctype.h>          // toupper
#include <limits.h>         // INT_MIN, INT_MAXop
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#include <stdint.h>         // intptr_t
#include <inttypes.h>       // PRId64/PRIu64, not avail in some MinGW headers.
#include <ctime>
#include <iomanip> 

struct LogUnit {
    const char* logText;
    LogLevel level;
    std::time_t timestamp;

    LogUnit(const char* text, LogLevel lvl) : logText(text), level(lvl), timestamp(time(0)) {}

    const char* LogUnitToString() const {
        static char buffer[512];
        const char* levelToString = nullptr;

        if (level == SYSINFO_LOG) { levelToString = "SYSTEM"; }
        else if (level == INFO_LOG) { levelToString = "INFO"; }
        else if (level == WARNING_LOG) { levelToString = "WARNING"; }
        else if (level == ERROR_LOG) { levelToString = "ERROR"; }
        else {
            levelToString = "UNKNOWN";
        }
        struct tm timeInfo;
        localtime_s(&timeInfo, &timestamp);
        char timBuffer[80];
        strftime(timBuffer, sizeof(timBuffer), "%Y-%m-%d %H:%M:%S", (&timeInfo));

        const char* logText = this->logText; 

        snprintf(buffer, sizeof(buffer), "[%s %s] %s", levelToString, timBuffer, logText);

        return buffer;
    }
};

class ConsoleGUI
{
public:
    char InputBuf[256];
    ImVector<LogUnit*> Items;
    ImVector<const char*> Commands;
    ImVector<char*> History;
    int HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter Filter;
    bool AutoScroll;
    bool ScrollToBottom;

    ConsoleGUI();
    ~ConsoleGUI();

    void ClearLog();
    void AddLog(const char* fmt, LogLevel level, ...) IM_FMTARGS(3);
    void AddLog(const char* fmt, LogLevel level, va_list args) IM_FMTARGS(3);
    void Draw(const char* title, bool* p_open);
    static void ShowConsole(bool* p_open);

private:
    //static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
    //static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
    static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
    //static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

};

