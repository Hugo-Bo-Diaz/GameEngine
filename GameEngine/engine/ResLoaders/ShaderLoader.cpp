#include <glad/glad.h>
#include "ShaderLoader.h"
#include "../Resources.h"

#include <fstream>
#include <sstream>
#include <string>

int ShaderLoader::LoadProgram(const char* prog)
{
	return 0;
}

Shader* ShaderLoader::LoadShader(const char* _file)
{
	
	std::string vertex_define = "#define VERTEX\n";
	std::string fragment_define = "#define FRAGMENT\n";

	std::string vertex_code;
	std::string fragment_code;
	std::ifstream f_shader_file;

	try
	{
		f_shader_file.open(_file);

		std::stringstream f_shader_stream;

		f_shader_stream << f_shader_file.rdbuf();

		f_shader_file.close();

		vertex_code = "#version 330 core\n" + vertex_define+ f_shader_stream.str();
		fragment_code = "#version 330 core\n" + fragment_define+ f_shader_stream.str();


	}
	catch (const std::exception&)
	{
		return nullptr;
	}
	const GLchar* v_shader_code = vertex_code.c_str();
	const GLchar* f_shader_code = fragment_code.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &v_shader_code, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		return nullptr;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &f_shader_code, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		return nullptr;
	}

	Shader* ret = new Shader();

	ret->bufferID = glCreateProgram();
	glAttachShader(ret->bufferID, vertex);
	glAttachShader(ret->bufferID, fragment);
	glLinkProgram(ret->bufferID);

	glGetProgramiv(ret->bufferID, GL_LINK_STATUS, &success);
	if (!success)
	{
		delete ret;
		return nullptr;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return ret;
}
