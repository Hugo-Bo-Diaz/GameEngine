#ifndef SHADER_LOADER__H
#define SHADER_LOADER__H

struct Shader;

class ShaderLoader
{
private:
	ShaderLoader() {};
	static int LoadProgram(const char* prog);
public:
	static Shader* LoadShader(const char* file);
};


#endif // !SHADER_LOADER__H
