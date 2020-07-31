#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader();
	~Shader();

	void CreateFromFiles(const char* VertexShaderLoc, const char* FragmentShaderLoc);
	
	//Comp shader experimental
	void CreateFromFiles(const char* CompShaderLoc);
	void CreateFromString(const char* VertexString, const char* FragmentString);

	GLuint UseShader() { glUseProgram(ShaderID); return ShaderID;}
	GLuint GetShaderID() { return ShaderID; }
	void ClearShader();

	GLuint GetUniformLocation(const char* UniformVar);
	
	void SetUniform4F(const char* UniformString, float X, float Y, float Z,float W);
	void SetUniform2F(const char* UniformString, float X, float Y);
	void SetUniform3F(const char* UniformString, float X, float Y, float z);
	void setUniform1F(const char* UniformString, float x);

	void SetUniformMatrix4F(const char* UniformString, std::vector<GLfloat> Matrixin, bool Transpose);
	void SetUniformMatrix4F(const char* UniformString, glm::mat4 &Matrixin, bool Transpose);

protected:

	//Addresses
	GLuint ShaderID;

	std::string ReadFile(const char* FileLocation);
	void CompileShaders(const char* VertexCode, const char*  FragmentCode);
	void CompileShader(const char* CompCode);
	
};

Shader::Shader()
{
	ShaderID = 0;
}

Shader::~Shader()
{
	ClearShader();
}

void Shader::SetUniformMatrix4F(const char* UniformString, std::vector<GLfloat> Matrixin, bool Transpose)
{
	GLuint a = 0;
	glUniformMatrix4fv(a = glGetUniformLocation(ShaderID, UniformString), 1, Transpose, &(Matrixin[0]));
	if (a == -1) {
		printf("%s is %i \n", UniformString, a);
	}
}

void Shader::SetUniformMatrix4F(const char* UniformString, glm::mat4 &Matrixin, bool Transpose)
{
	GLuint a = 0;
	glUniformMatrix4fv(a = glGetUniformLocation(ShaderID, UniformString), 1, Transpose, glm::value_ptr(Matrixin));
	if (a == -1)
	{
		printf("%s is %i \n", UniformString, a);
	}
}

void Shader::SetUniform4F(const char* UniformString, float X, float Y, float Z,float W)
{
	glUniform4f(glGetUniformLocation(ShaderID, UniformString), X, Y, Z,W);
}

void Shader::SetUniform2F(const char* UniformString, float X, float Y)
{
	GLuint a = 0;
	glUniform2f(a = glGetUniformLocation(ShaderID, UniformString),X, Y);
	if (a == -1)
	{
		printf("%s is %i\n", UniformString, a);
	}
}

void Shader::SetUniform3F(const char* UniformString, float X, float Y, float z)
{
	glUniform3f(glGetUniformLocation(ShaderID, UniformString), X, Y, z);
}

void Shader::setUniform1F(const char* UniformString, float x)
{
	glUniform1f(glGetUniformLocation(ShaderID, UniformString), x);
}

void Shader::CreateFromFiles(const char* VertexShaderLoc, const char* FragmentShaderLoc)
{
	std::string VertexString = ReadFile(VertexShaderLoc);
	std::string FragmentString = ReadFile(FragmentShaderLoc);

	const char* VertexCode = VertexString.c_str();
	const char* FragmentCode = FragmentString.c_str();

	CompileShaders(VertexCode, FragmentCode);
}

void Shader::CreateFromFiles(const char* CompShaderLoc)
{
	std::string CompString = ReadFile(CompShaderLoc);

	const char* CompCode = CompString.c_str();

	CompileShader(CompCode);
}

void Shader::CreateFromString(const char* VertexString, const char* FragmentString)
{
	CompileShaders(VertexString, FragmentString);
}

void Shader::ClearShader()
{
	if (ShaderID != 0)
	{
		glDeleteProgram(ShaderID);
	}
}

std::string Shader::ReadFile(const char* FileLocation)
{
	std::string Content = "";
	std::fstream FileStream(FileLocation, std::ios::in);

	if (!FileStream.is_open())
	{
		printf("Failed to open file location");
		return "";
	}

	std::string line = "";

	while (!FileStream.eof())
	{
		std::getline(FileStream, line);
		Content.append(line + "\n");
	}
	//std::cout << Content; //debugging
	FileStream.close();

	return Content;
}

GLuint Shader::GetUniformLocation(const char* UniformVar)
{
	GLuint UniformVarLoc = glGetUniformLocation(ShaderID, UniformVar);
	//printf("Checking %s Location: %u\n", UniformVar, (unsigned int)UniformVarLoc); //Debug
	return glGetUniformLocation(ShaderID, UniformVar);
}
void Shader::CompileShader(const char* CompCode)
{
	ShaderID = glCreateProgram();

	if (!ShaderID)
	{
		printf("Failed to create shader");
		return;
	}

	GLuint CompShader = glCreateShader(GL_COMPUTE_SHADER);

	if (0 == CompShader)
	{
		printf("Failed to initalise Compute shader");
		return;
	}

	glShaderSource(CompShader, 1, &CompCode, NULL);
	glCompileShader(CompShader);

	GLint result;
	char Log[512];
	glGetShaderiv(CompShader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{

		glGetShaderInfoLog(CompShader, 512, NULL, Log);
		printf("Failed to compile Compute shader: %s\n", Log);
		return;
	}

	glCompileShader(CompShader);

	glAttachShader(ShaderID, CompShader);
	glLinkProgram(ShaderID);

	glGetShaderiv(ShaderID, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		glGetShaderInfoLog(ShaderID, 512, NULL, Log);
		printf("Failed to attach shaders: %s\n", Log);
		return;
	}

	//glDeleteShader(CompShader);
}

void Shader::CompileShaders(const char* VertexCode, const char* FragmentCode)
{
	ShaderID = glCreateProgram();

	if (!ShaderID)
	{
		printf("Failed to create shader");
		return;
	}

	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);

	if (0 == VertexShader)
	{
		printf("Failed to initalise vertex shader");
		return;
	}

	glShaderSource(VertexShader, 1, &VertexCode, NULL);
	glCompileShader(VertexShader);

	GLint result;
	char Log[512];
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{

		glGetShaderInfoLog(VertexShader, 512, NULL, Log);
		printf("Failed to compile Vert shader: %s\n", Log);
		return;
	}

	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	if (0 == FragmentShader)
	{
		printf("Failed to create fragmentshader");
		return;
	}

	glShaderSource(FragmentShader, 1, &FragmentCode, NULL);
	glCompileShader(FragmentShader);

	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{

		glGetShaderInfoLog(FragmentShader, 512, NULL, Log);
		printf("Failed to compile FragmentShader: %s\n", Log);
		return;
	}

	glCompileShader(VertexShader);
	glCompileShader(FragmentShader);

	glAttachShader(ShaderID, VertexShader);
	glAttachShader(ShaderID, FragmentShader);
	glLinkProgram(ShaderID);

	glGetShaderiv(ShaderID, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		glGetShaderInfoLog(ShaderID, 512, NULL, Log);
		printf("Failed to attach shaders: %s\n", Log);
		return;
	}

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

}
