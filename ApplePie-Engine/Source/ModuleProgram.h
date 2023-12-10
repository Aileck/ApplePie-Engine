#ifndef __ModuleProgram_H__
#define __ModuleProgram_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	void LoadShaderFile();
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
	char* LoadShaderSource(const char* shader_file_name, bool is_a_file);

	unsigned CompileShader(unsigned type, const char* source);

	bool CleanUp();

	unsigned program;
};

#endif // __ModuleProgram_H__