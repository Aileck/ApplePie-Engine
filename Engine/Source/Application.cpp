#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleTimer.h"
#include "ModuleOpenGL.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleImGUI.h"
#include "ModuleRenderExercise.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleLoadModel.h"
#include "ConsoleGUI.h"


using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(timer = new ModuleTimer());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(render = new ModuleOpenGL());
	modules.push_back(input = new ModuleInput());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(debugDraw = new ModuleDebugDraw());
	modules.push_back(texture = new ModuleTexture()); 
	modules.push_back(editor = new ModuleImGUI());
	//modules.push_back(triangle = new ModuleRenderExercise());
	modules.push_back(modelLoader = new ModuleLoadModel());

}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();
	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Start();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}

void Application::WriteIntoLog(LogLevel level, const char* logText,  ...)
{
	va_list args;
	va_start(args, logText);
	editor->GetConsole()->AddLog(logText, level,args);
	va_end(args);
}

void Application::OpenBrowser(const char* url)
{
	HINSTANCE result = ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);

	if ((intptr_t)result <= 32) {

		DWORD error = GetLastError();
		WriteIntoLog(ERROR_LOG, reinterpret_cast<LPSTR>(error));
		WriteIntoLog(ERROR_LOG, "Cannot open %s", url);
	}
}
