#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class ConsoleGUI;
class ConfigurationGUI;
class PropertiesGUI;

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
	bool CleanUp();

	void DrawMainMenu();

	ConsoleGUI* GetConsole() { return logConsole; };
	

private:
	ConsoleGUI* logConsole = nullptr;
	ConfigurationGUI* conf = nullptr;
    PropertiesGUI* gometryProperties = nullptr;

	bool show_tool_about = false;

	bool quit = false;
};

#endif
