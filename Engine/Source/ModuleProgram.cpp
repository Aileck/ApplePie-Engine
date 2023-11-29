#include "ModuleProgram.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL\glew.h"

const char* vertexShaderMVP = R"(
	#version 430
	layout(location=0) in vec3 my_vertex_position;
	layout(location = 0)uniform mat4 model;
	layout(location = 1) uniform mat4 view;
	layout(location = 2) uniform mat4 proj;
	void main()
	{
	 gl_Position = proj * view * model * vec4(my_vertex_position, 1.0);
	}
)";


const char* vertexShaderHW = R"(
    #version 330
    layout(location=0) in vec3 my_vertex_position;
    void main()
    {
        gl_Position = vec4(my_vertex_position, 1.0);
    }
)";

const char* fragmentShaderHW = R"(
    #version 330
    out vec4 color;
    void main()
    {
        color = vec4(1.0, 0.0, 0.0, 1.0);
    }
)";

ModuleProgram::ModuleProgram()
{
}

ModuleProgram::~ModuleProgram()
{
	CleanUp();
}

bool ModuleProgram::Init() {
	char *vtx_shader, *frg_shader = nullptr;
	
	//const char* VerteixShaderFile = App->CreateFilePath(SHADER_PATH, "triangleExerciceVertexTexture.glsl");
	//const char* FragmentShaderFile = App->CreateFilePath(SHADER_PATH, "triangleExerciceFragmentTexture.glsl");
	const char* VerteixShaderFile = App->CreateFilePath(SHADER_PATH, "triangleExerciceVertex.glsl");
	const char* FragmentShaderFile = App->CreateFilePath(SHADER_PATH, "triangleExerciceFragment.glsl");
	vtx_shader = LoadShaderSource(VerteixShaderFile, true);
	frg_shader = LoadShaderSource(FragmentShaderFile, true);  

	unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vtx_shader);
	unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, frg_shader);

	glObjectLabel(GL_SHADER, vertex, -1, VerteixShaderFile);
	glObjectLabel(GL_SHADER, fragment, -1, FragmentShaderFile);


	this->program = CreateProgram(vertex, fragment);

	delete[] (VerteixShaderFile, FragmentShaderFile);
	return true;

}

unsigned ModuleProgram::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{

	unsigned program_id = glCreateProgram();

	//Finally we must create program object containing both shaders
	glAttachShader(program_id, vtx_shader);
	glAttachShader(program_id, frg_shader);

	//Now we can attach both shaders 
	glLinkProgram(program_id);
	int res;

	//Similar to shader compilation we can check linking errors:
	glGetProgramiv(program_id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetProgramInfoLog(program_id, len, &written, info);
			LOG("Program Log Info: %s", info);
			free(info);
		}
	}

	//Note!!!: after linking we can delete shaders.
	glDeleteShader(vtx_shader);
	glDeleteShader(frg_shader);
	return program_id;
}

char* ModuleProgram::LoadShaderSource(const char* shader_file_name, bool is_a_file = true)
{
	if (!is_a_file)
		return  const_cast<char*>(shader_file_name);


	char* data = nullptr;
	FILE* file = nullptr;

	if(shader_file_name)
		fopen_s(&file, shader_file_name, "rb");

	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		data = (char*)malloc(size + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
	}
	else {

		LOG("Cannot find file: %s", shader_file_name);
		exit(0);

	}
	return data;
}

unsigned ModuleProgram::CompileShader(unsigned type, const char* source)
{
	///For compiling both shaders we need to create two shader objects
	unsigned shader_id = glCreateShader(type);
	LOG("Source: %s", &source);
	//Once created, we can attach data and compile each one:
	glShaderSource(shader_id, 1, &source, 0);
	glCompileShader(shader_id);

	//And, of course, we can check if compilation went ok and errors
	int res = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);

	if (res == GL_FALSE)
	{
		int len = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetShaderInfoLog(shader_id, len, &written, info);
			LOG("Log Info: %s", info);
			free(info);
		}
	}
	return shader_id;
}

bool ModuleProgram::CleanUp()
{
	glDeleteProgram(this->program);
	return true;
}