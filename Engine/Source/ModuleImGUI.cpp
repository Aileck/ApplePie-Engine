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

ModuleImGUI::ModuleImGUI()
{
}

ModuleImGUI::~ModuleImGUI()
{
}

bool ModuleImGUI::Init()
{
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->window, App->GetOpenGL()->context);
    ImGui_ImplOpenGL3_Init(nullptr);

    //logConsole = new ConsoleGUI();

    return true;
}

update_status ModuleImGUI::PreUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->GetWindow()->window);

    //if (ImGui::Button("Save")) {}
    //static char userInput[128] = "Hello, world!";
    //float *userFloat = new float(0.0f);
    //ImGui::InputText("string", userInput, IM_ARRAYSIZE(userInput));
    //ImGui::SliderFloat("float", userFloat, 0.0f, 1.0f);


    return UPDATE_CONTINUE;
}

update_status ModuleImGUI::Update()
{
    return UPDATE_CONTINUE;
    //ImGui::ShowDemoWindow();
    //bool logOpen = true;
    //logConsole->Draw("Debug/Error log", &(logOpen));

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    
    //ImGui_ImplSDL2_ProcessEvent(App->GetInput()->currentEvent);

    return UPDATE_CONTINUE;
}

update_status ModuleImGUI::PostUpdate()
{

    return UPDATE_CONTINUE;
}

void ModuleImGUI::DrawConsole()
{

}

bool ModuleImGUI::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    return false;
}
