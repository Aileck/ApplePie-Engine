#pragma once

#include "backends\imgui.h"
#include "backends\imgui_impl_sdl2.h"
#include "backends\imgui_impl_opengl3.h"

class ConfigurationGUI
{
public:
	ConfigurationGUI();
	void Draw();
	void DrawMainMenu();

private:
	bool hideEditor = false;

	bool hasNoFinishedInput = false;

	bool editorNoBorder = false;
	bool editorFullcreen = false;

	bool setWindow;
	bool resetWindow;
	int temporalHeight;
	int temporalWidth;
};

