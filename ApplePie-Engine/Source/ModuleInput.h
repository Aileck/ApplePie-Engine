#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"

#include <list>

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
	bool CheckIfMouseDown(Uint8 mouseEvent);
	int GetMouseWheelValue() const { return wheelValueMouse; };
	int GetDragDistance() const { return dragDistance; };



private:

	void HandleDropEvent(SDL_Event event);

	const Uint8 *keyboard = nullptr;

	// MouseEvents
	std::list<Uint8> pressedButtonsMouse;
	Sint32 wheelValueMouse;
	float startX;
	bool dragging;
	int dragDistance = 0;



};