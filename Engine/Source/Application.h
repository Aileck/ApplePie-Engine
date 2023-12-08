#pragma once

#include <list>
#include "Globals.h"
#include "Module.h"


class ModuleOpenGL;
class ModuleWindow;
class ModuleTexture;
class ModuleInput;
class ModuleRenderExercise;
class ModuleProgram;
class ModuleImGUI;
class ModuleDebugDraw;
class ModuleCamera;
class ModuleLoadModel;
class ModuleTimer;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() { return render; }
    ModuleWindow* GetWindow() { return window; }
    ModuleInput*  GetInput() { return input; }
    ModuleProgram* GetProgram() { return program; }
    ModuleRenderExercise* GetTriangle() { return triangle; }
    ModuleImGUI* GetEditor() { return editor; }
    ModuleDebugDraw* GetDebugDraw() { return debugDraw; }
    ModuleCamera* GetCamera() { return camera; }
    ModuleTexture* GeTexture() { return texture; }
    ModuleLoadModel* GetModelLoader() { return modelLoader; }
    ModuleTimer* GetModuleTimer() { return timer; }

    void WriteIntoLog(LogLevel level, const char* logText, ...);
    void OpenBrowser(const char* url);

private:
    // Modules
    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleProgram* program = nullptr;
    ModuleRenderExercise* triangle = nullptr;
    ModuleImGUI* editor = nullptr;
    ModuleDebugDraw* debugDraw = nullptr;
    ModuleCamera* camera = nullptr;
    ModuleTexture* texture = nullptr;
    ModuleLoadModel* modelLoader = nullptr;
    ModuleTimer* timer = nullptr;

    std::list<Module*> modules;

    // Components

};

extern Application* App;
