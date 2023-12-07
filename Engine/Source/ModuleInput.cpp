#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleOpenGL.h"
#include "ModuleLoadModel.h"
#include "FileComponent.h"

#include "backends\imgui.h"
#include "backends\imgui_impl_sdl2.h"
#include "backends\imgui_impl_opengl3.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleInput::PreUpdate()
{
    SDL_Event sdlEvent;
    wheelValueMouse = 0;
    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        // #editor module
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

        // end editor module
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->GetOpenGL()->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
                if (sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE) {
                    return UPDATE_STOP;
                }
                break;
            case SDL_KEYDOWN:

                //return UPDATE_CONTINUE;

                break;

            case SDL_KEYUP:
                //return UPDATE_CONTINUE;
                break;

            case SDL_DROPFILE:
                HandleDropEvent(sdlEvent);
                //return UPDATE_CONTINUE;
                break;

            case SDL_MOUSEBUTTONDOWN:
            {
                auto it = std::find(pressedButtonsMouse.begin(), pressedButtonsMouse.end(), sdlEvent.button.button);
                if (it == pressedButtonsMouse.end()) {
                    pressedButtonsMouse.push_back(sdlEvent.button.button);
                }
                break;
            }

            case SDL_MOUSEBUTTONUP:
            {
                auto it = std::find(pressedButtonsMouse.begin(), pressedButtonsMouse.end(), sdlEvent.button.button);
                if (it != pressedButtonsMouse.end()) {
                    pressedButtonsMouse.erase(it);
                }
                break;
            }

            case SDL_MOUSEWHEEL:
            {
                wheelValueMouse = sdlEvent.wheel.y;
                break;
            }
        }

    }
    keyboard = SDL_GetKeyboardState(NULL);

    //if (keyboard[SDL_SCANCODE_W]) {
    //    LOG("Press W\n");
    //}
    //if (keyboard[SDL_SCANCODE_Q]) {
    //    LOG("Press Q\n");
    //}

    //LOG("Keyboard value: %hhu\n", *keyboard);

    return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleInput::Update()
{


    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

bool ModuleInput::CheckIfPressed(SDL_Scancode keycode) {

    
    if (keyboard[keycode]) {
        //LOG("Press");
    }
    return keyboard[keycode];
}

bool ModuleInput::CheckIfMouseDown(Uint8 mouseEvent)
{
    auto it = std::find(pressedButtonsMouse.begin(), pressedButtonsMouse.end(), mouseEvent);

    if (it != pressedButtonsMouse.end()) {
        return true;
    }

    return false;
}

int ModuleInput::GetMouseWheelValue()
{
    LOG("Weel: %i", wheelValueMouse);
    return wheelValueMouse;
}


void ModuleInput::HandleDropEvent(SDL_Event event)
{
    char* dropped_filedir = event.drop.file;
    char filePath[256];
    char fileName[256];
    char fileExtension[256];

    FileComponent::AnalyseFilePath(dropped_filedir, filePath, fileName, fileExtension);

    if (strcmp(fileExtension,"gltf") == 0) {
        App->GetModelLoader()->LoadExteriorModel(dropped_filedir);
    }
    else {
        LOG("Unsupported extension: %s", fileExtension);
    }

    SDL_free(dropped_filedir);
}


