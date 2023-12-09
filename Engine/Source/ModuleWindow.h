#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();
	// Called before quitting
	bool CleanUp();

	void SetCurrentWindowSize();
	void SetWindowSize(int height, int width);
	void SetFullscreen(bool active);
	void SetBorderless(bool active);

	inline SDL_Window* GetWindow() const { return window; }
	inline int GetCurrentHeight() const { return currentHeight; }
	inline int GetCurrentWidth() const { return currentWidth; }
	

private:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	int currentHeight;
	int currentWidth;
	
};

#endif // __ModuleWindow_H__