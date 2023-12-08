#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleCamera.h"
#include "ModuleDebugDraw.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL\glew.h"
#include "backends\imgui.h"
#include "backends\imgui_impl_sdl2.h"
#include "backends\imgui_impl_opengl3.h"

ModuleOpenGL::ModuleOpenGL()
{

}

// Destructor
ModuleOpenGL::~ModuleOpenGL()
{
}

void __stdcall OurOpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	const char* tmp_source = "", * tmp_type = "", * tmp_severity = "";
	switch (source) {
		case GL_DEBUG_SOURCE_API: tmp_source = "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: tmp_source = "Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: tmp_source = "Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: tmp_source = "Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION: tmp_source = "Application"; break;
		case GL_DEBUG_SOURCE_OTHER: tmp_source = "Other"; break;
	};
	switch (type) {
		case GL_DEBUG_TYPE_ERROR: tmp_type = "Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: tmp_type = "Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: tmp_type = "Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY: tmp_type = "Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE: tmp_type = "Performance"; break;
		case GL_DEBUG_TYPE_MARKER: tmp_type = "Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP: tmp_type = "Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP: tmp_type = "Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER: tmp_type = "Other"; break;
	};
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH: tmp_severity = "high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: tmp_severity = "medium"; break;
		case GL_DEBUG_SEVERITY_LOW: tmp_severity = "low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: tmp_severity = "notification"; break;
	};
	App->WriteIntoLog(SYSINFO_LOG, "<Source:%s> <Type:%s> <Severity:%s> <ID:%d> <Message:%s>\n", tmp_source, tmp_type, tmp_severity, id, message);
}

// Called before render is available
bool ModuleOpenGL::Init()
{

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bit

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	context = SDL_GL_CreateContext(App->GetWindow()->window);

	GLenum err = glewInit();



	//Initialize render pipeline options
	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glDisable(GL_SCISSOR_TEST);    // We are not going to use scissor test for now.
	glDisable(GL_STENCIL_TEST);    //  We are not going to use stencil test for now

	glFrontFace(GL_CCW); // Front faces will be counter clockwise

	// Debug
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&OurOpenGLErrorFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,true);
	return true;
}

bool ModuleOpenGL::Start()
{
	App->WriteIntoLog(SYSINFO_LOG, "Creating Renderer context");
	App->WriteIntoLog(SYSINFO_LOG, "Using Glew %s", glewGetString(GLEW_VERSION));
	App->WriteIntoLog(SYSINFO_LOG, "Vendor: %s", glGetString(GL_VENDOR));
	App->WriteIntoLog(SYSINFO_LOG, "Renderer: %s", glGetString(GL_RENDERER));
	App->WriteIntoLog(SYSINFO_LOG, "OpenGL version supported %s", glGetString(GL_VERSION));
	App->WriteIntoLog(SYSINFO_LOG, "GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	return true;
}

update_status ModuleOpenGL::PreUpdate()
{
	//a.Setup glViewport to 0, 0, window_width, window_height if window is resized :
	//	i.Use SDL_GetWindowSize
	//b.glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//c.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
	
	//a
	int h, w;
	SDL_GetWindowSize(App->GetWindow()->window, &w, &h);

	if (WindowResized(w, h)) {
		glViewport(0,0,w,h);
	}
	//b
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	//c
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleOpenGL::Update()
{

	return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::PostUpdate()
{
	SDL_GL_SwapWindow(App->GetWindow() ->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleOpenGL::CleanUp()
{
	App->WriteIntoLog(SYSINFO_LOG, "Destroying renderer");
	SDL_GL_DeleteContext(context);
	//Destroy window

	return true;
}

bool ModuleOpenGL::WindowResized(unsigned width, unsigned height)
{
	if (App->GetWindow()->currentWidth != width || App->GetWindow()->currentHeight != height) {
		App->GetWindow()->SetCurrentWindowSize();
		App->WriteIntoLog(SYSINFO_LOG, "Resized");
		return true;
	}
	else {
		return false;
	}
}

void* ModuleOpenGL::GetContext()
{
	return context;
}

