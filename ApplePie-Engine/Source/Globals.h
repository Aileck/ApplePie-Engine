#pragma once

#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum LogLevel {
    SYSINFO_LOG,
    INFO_LOG,
    WARNING_LOG,
    ERROR_LOG
};

// Configuration -----------
#define CURRENT_VERSION "v0.01 Beta"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define TEXTURE_PATH R"(2D\Texture\)"
#define MODEL_PATH R"(3D\)"
#define SHADER_PATH R"(Shader\)"
#define FULLSCREEN false
#define VSYNC true
#define TITLE "Apple Pie Engine"