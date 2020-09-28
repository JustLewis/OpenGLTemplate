//Time to make some funky stuff
#include <iostream>

#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Template/Window.h"
#include "../Template/Shader.h"
#include "ComputeParticles.h"

Window window;
Shader shader;
//From Kronos - Debugging function protype. (Full function below main)
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

int main()
{
	window = Window(1920, 1080);
	//window = Window(1920*2, 1080*2);
	window.Initialise();

	//Debug stuff.
	glDebugMessageCallback(MessageCallback, 0);
	//glEnable(GL_DEBUG_OUTPUT); //Uncomment if you want some debugging information

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendColor(0.4, 0.4, 0.0, 0.3);

	Shader CompShader;
	CompShader.CreateComputeShaderFromFile("Shader/ParticleComputeShader.glsl");
	CompShader.UseShader();

	ComputeParticles Party;
	Party.CreateParticles(1000000); //1 million. (Maximum is 25 million before crashing.)

	GLfloat Posx = 0.0f, Posy = 0.0f;

	glPointSize(1);
	//glEnable(GL_DEPTH_TEST);
	
	shader.CreateFromFiles("Shader/2DShader.vert", "Shader/2DShader.frag");

	while (!window.GetWindowShouldClose())
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.01f, 0.0f, 0.02f, 1.0f);

		Posx += window.GetMouseXChange() * 0.001;
		Posy += window.GetMouseYChange() * 0.001;

		if (Posx < -1.0f) { Posx = 1.0f; }
		if (Posy < -1.0f) { Posy = 1.0f; }
		if (Posx > 1.0f) { Posx = -1.0f; }
		if (Posy > 1.0f) { Posy = -1.0f; }
		CompShader.UseShader();
		CompShader.SetUniform2F("MousePosition", Posx, Posy);

		Party.Dispatch();

		shader.UseShader();
		shader.SetUniform4F("ObjColour", 0.990f, 0.9910f, 0.9910f, 0.10f);

		//mesh.RenderMesh();
		Party.Render();

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