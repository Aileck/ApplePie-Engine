#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	bool CheckIfPressed(SDL_Scancode keycode);
	void HandleDropEvent(SDL_Event event);

private:
	const Uint8 *keyboard = nullptr;
};