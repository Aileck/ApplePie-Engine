#pragma once

#include "backends\imgui.h"
#include "backends\imgui_impl_sdl2.h"
#include "backends\imgui_impl_opengl3.h"

struct FrameRateData;

class ConfigurationGUI
{
public:
	void Draw(FrameRateData frames);
	void DrawMainMenu();

private:
	bool hideEditor = false;
};

