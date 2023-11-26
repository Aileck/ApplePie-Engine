#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleImGUI.h"
#include "ModuleRenderExercise.h"
#include "ModuleDebugDraw.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleLoadModel.h"

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(render = new ModuleOpenGL());
	modules.push_back(input = new ModuleInput());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(editor = new ModuleImGUI());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(debugDraw = new ModuleDebugDraw());
	modules.push_back(texture = new ModuleTexture()); 

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

void Application::WriteIntoLog(char* logText, LogLevel level)
{
	editor->logConsole->AddLog(logText,level);
}

const char* Application::CreateFilePath(const char* address, const char* fileName) {
	size_t fullPathSize = strlen(address) + strlen(fileName) + 2;

	char* fullPath = new char[fullPathSize];

	strcpy_s(fullPath, fullPathSize, address);
	strcat_s(fullPath, fullPathSize, fileName);

	return fullPath;
}

const wchar_t* Application::CreateWideFilePath(const char* address, const char* fileName) {
	size_t addressLen = strlen(address) + 1;
	size_t fileNameLen = strlen(fileName) + 1;

	wchar_t* wideAddress = new wchar_t[addressLen];
	wchar_t* wideFileName = new wchar_t[fileNameLen];

	mbstowcs_s(nullptr, wideAddress, addressLen, address, _TRUNCATE);
	mbstowcs_s(nullptr, wideFileName, fileNameLen, fileName, _TRUNCATE);

	size_t fullPathSize = wcslen(wideAddress) + wcslen(wideFileName) + 2; 

	wchar_t* fullPath = new wchar_t[fullPathSize];

	wcscpy_s(fullPath, fullPathSize, wideAddress);

	if (fullPath[wcslen(fullPath) - 1] != L'/') {
		wcscat_s(fullPath, fullPathSize, L"/");
	}

	wcscat_s(fullPath, fullPathSize, wideFileName);

	delete[] wideAddress;
	delete[] wideFileName;

	return fullPath;
}

//void Application::WriteIntoLog() {
//
//}
