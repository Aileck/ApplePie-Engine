#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "ConsoleGUI.h"

class ModuleImGUI :
    public Module
{
public:
	ModuleImGUI();
	~ModuleImGUI();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	void DrawConsole();

	bool CleanUp();

	ConsoleGUI* logConsole = nullptr;
};

#endif
