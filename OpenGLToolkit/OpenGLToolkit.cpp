//Time to make some funky stuff
#include <iostream>

#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //not sure we need this.

#include "../Template/Window.h"
#include "../Template/Shader.h"
#include "../Template/Mesh.h"


Window window;
Shader shader;
Mesh mesh;

bool TwoDimensions = false;

const char* VertexShader2D = "../Template/Shaders/Basic2DShader.vert";
const char* FragmentShader2D = "../Template/Shaders/Basic2DShader.frag";

const char* VertexShader3D = "../Template/Shaders/Basic3DShader.vert";
const char* FragmentShader3D = "../Template/Shaders/Basic3DShader.frag";

void GLAPIENTRY //From Kronos - Debugging function
MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

int main()
{
	window = Window(1920, 1080);
	window.Initialise();

	glDebugMessageCallback(MessageCallback, 0);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);

	if (TwoDimensions)//Working in 2D
	{
		
		shader.CreateFromFiles(VertexShader2D, FragmentShader2D);
		shader.UseShader();

		mesh = Mesh();
		mesh.CreateTriangle();

		while (!window.GetWindowShouldClose())
		{
			glfwPollEvents();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.01f, 0.0f, 0.02f, 1.0f);

			shader.SetUniform4F("ObjColour", 0.0f, 0.0f, 1.0f, 1.0f);

			mesh.RenderMesh();

			window.SwapBuffers();
		}
	}
	else //working in 3D
	{
		shader.CreateFromFiles(VertexShader3D, FragmentShader3D);
		shader.UseShader();

		glm::mat4 ProjectionMatrix = glm::perspective(45.0f, 1920.0f / 1080.0f, 0.1f, 100.0f);

		glm::mat4 ViewMatrix = glm::mat4(1.0f);
		glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,-2.0f));

		shader.SetUniformMatrix4F("Projection", ProjectionMatrix,false);
		shader.SetUniformMatrix4F("View", ViewMatrix,false);
		shader.SetUniformMatrix4F("Model", ModelMatrix,false);

		mesh = Mesh();
		mesh.CreateTriangle();

		while (!window.GetWindowShouldClose())
		{
			glfwPollEvents();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.01f, 0.0f, 0.02f, 1.0f);

			shader.SetUniform4F("ObjColour", 0.0f, 0.0f, 1.0f, 1.0f);

			mesh.RenderMesh();

			window.SwapBuffers();
		}
	}
}