#include "ModuleImGUI.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "Globals.h"
#include "Application.h"
#include "SDL.h"
#include "GL\glew.h"
#include "backends\imgui.h"
#include "backends\imgui_impl_sdl2.h"
#include "backends\imgui_impl_opengl3.h"

using namespace ImGui;
ModuleImGUI::ModuleImGUI()
{
}

ModuleImGUI::~ModuleImGUI()
{
}

bool ModuleImGUI::Init()
{
    CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->context);
    ImGui_ImplOpenGL3_Init(nullptr);

    logConsole = new ConsoleGUI();

    return true;
}

update_status ModuleImGUI::PreUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->GetWindow()->window);
    NewFrame();
    Text("Hello, wolrd %d", 123);

    bool logOpen = true;
    logConsole->Draw("Debug/Error log", &(logOpen));

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return UPDATE_CONTINUE;
}

update_status ModuleImGUI::Update()
{



    return UPDATE_CONTINUE;
}

update_status ModuleImGUI::PostUpdate()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        UpdatePlatformWindows();
        RenderPlatformWindowsDefault();
    }
    SDL_GL_MakeCurrent(App->GetWindow()->window, App->GetOpenGL()->context);

    ;
    return UPDATE_CONTINUE;
}

void ModuleImGUI::DrawConsole()
{

}

bool ModuleImGUI::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    DestroyContext();
    return false;
}
