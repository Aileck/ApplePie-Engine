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

#include "ConsoleGUI.h"
#include "ConfigurationGUI.h"

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
    conf = new ConfigurationGUI();

    return true;
}

update_status ModuleImGUI::PreUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->GetWindow()->window);
    NewFrame();
    //ShowDemoWindow();

    // MAIN MENU
    DrawMainMenu();

    // LOG WINDOW
    bool logOpen = true;
    logConsole->Draw("Debug/Error log", &(logOpen));

    // CONF
    conf->Draw();

    // DEMO
    ShowDemoWindow();
    

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (quit) {
        return UPDATE_STOP;
    }
    return UPDATE_CONTINUE;
}

update_status ModuleImGUI::Update()
{

    return UPDATE_CONTINUE;
}

update_status ModuleImGUI::PostUpdate()
{
    ImGuiIO& io = GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        UpdatePlatformWindows();
        RenderPlatformWindowsDefault();
    }
    SDL_GL_MakeCurrent(App->GetWindow()->window, App->GetOpenGL()->context);
    EndFrame();
    return UPDATE_CONTINUE;
}

void ModuleImGUI::DrawMainMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Editor"))
        {
            if (ImGui::MenuItem("Editor windows", NULL, true)) {
                SDL_HideWindow(App->GetWindow()->window);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("About us")) {
                
            }
            if (ImGui::MenuItem("Find us")) {
                App->OpenBrowser("https://github.com/Aileck/ApplePie-Engine");
            } 
            ImGui::Separator();
            if (ImGui::MenuItem("Quit")) 
            {
                quit = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

}

bool ModuleImGUI::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    DestroyContext();
    return false;
}
