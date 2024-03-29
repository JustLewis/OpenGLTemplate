//Time to make some funky stuff
#include <iostream>

#include<glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //not sure we need this.

#include "../Template/Window.h"
#include "../Template/Shader.h"
#include "../Template/Mesh.h"

#define TwoDimensions true; //False for 3D.

Window window;
Shader shader;
Mesh mesh;

//From Kronos - Debugging function protype. (Full function below main)
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

int main()
{
	window = Window(1920, 1080);
	//window = Window(1920*2, 1080*2);
	window.Initialise();

	glPointSize(1);
	glEnable(GL_DEPTH_TEST);

	//Debug stuff. Uncomment the below 2 functions for debug information
	//glDebugMessageCallback(MessageCallback, 0);
	//glEnable(GL_DEBUG_OUTPUT);

#if TwoDimensions
	shader.CreateFromFiles("../Template/Shaders/Basic2DShader.vert", "../Template/Shaders/Basic2DShader.frag");
	shader.UseShader();
#else
	shader.CreateFromFiles("../Template/Shaders/Basic3DShader.vert", "../Template/Shaders/Basic3DShader.frag");
	shader.UseShader();
	glm::mat4 ProjectionMatrix = glm::perspective(45.0f, 1920.0f / 1080.0f, 0.1f, 100.0f);
	glm::mat4 ViewMatrix = glm::mat4(1.0f);
	glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));

	shader.SetUniformMatrix4F("Projection", ProjectionMatrix, false);
	shader.SetUniformMatrix4F("View", ViewMatrix, false);
	shader.SetUniformMatrix4F("Model", ModelMatrix, false);
#endif

	mesh = Mesh();
	mesh.CreateTriangle();

	while (!window.GetWindowShouldClose())
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.01f, 0.0f, 0.02f, 1.0f);
		shader.UseShader();
		shader.SetUniform4F("ObjColour", 0.0f, 0.0f, 1.0f, 1.0f);

		mesh.RenderMesh();

		window.SwapBuffers();
	}

}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	//main stuff we want to know.
	std::cout << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "**ERROR**" : "") << ". Message = " << message << std::endl;
	//additional information included in the message.
	std::cout << "(Additional information) \ntype = 0x" << type << ". severity = 0x" << severity << ".\n\n";

}