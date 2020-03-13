//Time to make some funky stuff
#include <iostream>

#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "../Template/Window.h"
#include "../Template/Shader.h"
#include "../Template/Mesh.h"

Window window;
Shader shader;
Mesh* mesh;

const char* VertexShader = "../Template/Shaders/Basic2DShader.vert";
const char* FragmentShader = "../Template/Shaders/Basic2DShader.frag";

void GLAPIENTRY //From Kronos; Debugging function
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


	shader.CreateFromFiles(VertexShader, FragmentShader);
	shader.UseShader();

	mesh = new Mesh();
	mesh->CreateTriangle();

	while (!window.GetWindowShouldClose())
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.01f, 0.0f, 0.02f, 1.0f);

		shader.SetUniform4F("ObjColour", 0.0f, 0.0f, 1.0f, 1.0f);

		mesh->RenderMesh();

		window.SwapBuffers();
	}

	delete mesh;
	mesh = nullptr;

}