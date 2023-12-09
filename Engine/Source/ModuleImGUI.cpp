#include "ModuleImGUI.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTimer.h"
#include "ModuleLoadModel.h"
#include "Globals.h"
#include "Application.h"
#include "SDL.h"
#include "GL\glew.h"
#include "backends\imgui.h"
#include "backends\imgui_impl_sdl2.h"
#include "backends\imgui_impl_opengl3.h"

#include "ConsoleGUI.h"
#include "ConfigurationGUI.h"
#include "PropertiesGUI.h"

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
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->GetWindow(), App->GetOpenGL()->context);
    ImGui_ImplOpenGL3_Init(nullptr);

    logConsole = new ConsoleGUI();
    conf = new ConfigurationGUI();
    gometryProperties = new PropertiesGUI();

    return true;
}

update_status ModuleImGUI::PreUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->GetWindow()->GetWindow());
    NewFrame();
    //ShowDemoWindow();
    // GET FRAME
    frameRates.AddNewFrame(App->GetTimer()->GetFrame());

    // MAIN MENU
    DrawMainMenu();

    // LOG WINDOW
    bool logOpen = true;
    logConsole->Draw("Debug/Error log", &(logOpen));

    // CONF
    conf->Draw(frameRates);

    // GEOMETRY
    gometryProperties->Draw(App->GetModelLoader()->GetModel());
    // DEMO
    ShowDemoWindow();
    

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());

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
    SDL_GL_MakeCurrent(App->GetWindow()->GetWindow(), App->GetOpenGL()->context);
    EndFrame();
    return UPDATE_CONTINUE;
}

void ModuleImGUI::DrawMainMenu()
{

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Editor"))
        {
            if (ImGui::MenuItem("About us", NULL)) {
                show_tool_about = !show_tool_about;
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

    if (show_tool_about && Begin("About us", &show_tool_about))
    {
        ImGui::Text("%s is an Unity like 3D game engine. ", TITLE);
        ImGui::Text("Developed by Xinyu Jiang (xinyujiang721@gmail.com). Licensed under the MIT License.");
        ImGui::Separator();
        ImGui::Text("Current version %s", CURRENT_VERSION);
        ImGui::Separator();
        ImGui::Text("In the Apple Pie Engine, the apple metaphorizes the forbidden fruit in Eden. ");
        ImGui::Text("I chose to comply and refrain from taking a bite (using Unity without delving into its principles). ");
        ImGui::Text("Now, I choose to defy the will, picking and coocking it into a tasty apple pie, and pursue knowledge and freedom.");
        ImGui::Separator();
        ImGui::Text("Libraries:");
        ImGui::Text("- SDL 2.0");
        ImGui::Text("- ImGUI 1.89");
        ImGui::Text("- glew 2.1.0");
        ImGui::Text("- DirectTex 2.0.1");
        ImGui::Text("- MathGeoLib");
        ImGui::Text("- Tiny_GLTF 2.0");

        ImGui::End();
    }

}

bool ModuleImGUI::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    DestroyContext();
    return false;
}
