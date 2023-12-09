#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include <cmath>

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		// Initialize with default value window 
		int width = SCREEN_WIDTH;
		int height = SCREEN_HEIGHT;
		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

		// Try to adapt monitor size
		int displayIndex = 0; 
		SDL_DisplayMode dm;
		if (SDL_GetCurrentDisplayMode(displayIndex, &dm) != 0) {
			SDL_Quit();
			return -1;
		}

		// Try to not to fulfill all
		width = std::ceil(dm.w - dm.w * 0.1);
		height = std::ceil(dm.h - dm.h * 0.1);

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		currentHeight = height;
		currentWidth = width;

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

//void ModuleWindow::SetCurrentWindowSize()
//{
//	int h;
//	int w;
//
//	SDL_GetWindowSize(App->GetWindow()->window, &w, &h);
//
//	currentHeight = h;
//	currentWidth = w;
//}

void ModuleWindow::SetWindowSize(int height, int width)
{
	SDL_SetWindowSize(window, width, height);
}

void ModuleWindow::SetFullscreen(bool active)
{
	if (active) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else {
		SDL_SetWindowFullscreen(window, 0);
	}
	
}

void ModuleWindow::SetBorderless(bool active)
{
	if (active)
	{
		SDL_SetWindowBordered( window, SDL_FALSE);
	}
	else {
		SDL_SetWindowBordered(window, SDL_TRUE);
	}
}


