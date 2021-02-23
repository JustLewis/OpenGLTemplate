//Time to make some funky stuff
#include <iostream>

#include<glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //not sure we need this.

#include "../Template/Window.h"
#include "../Template/Shader.h"
#include "../Template/Mesh.h"
#include "ComputeTexture.h"

Window window;
Shader shader;
Shader compShader;
ComputeTexture Texture;
Mesh mesh = Mesh();

glm::vec2 Posa;
glm::vec2 Posb;
glm::vec2 Posc;
glm::vec2 Posd;

GLuint TexWidth = 400;
GLuint TexHeight = 400;

//From Kronos - Debugging function protype. (Full function below main)
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

void PlayerInput();
float BezForShader(float t, float wa, float wb, float wc, float wd);
void SplineOneAttempt();
void SplineTwoAttempt();
void Remake(float t,GLuint BufferIn);

bool RemakeBuffer = true;

std::vector<GLfloat> Points;

int main()
{
	window = Window(1920, 1080);
	//window = Window(1920*2, 1080*2); //4k
	window.Initialise();

	//glPointSize(1);
	//glEnable(GL_DEPTH_TEST);

	//Debug stuff. Uncomment the below 2 functions for debug information
	glDebugMessageCallback(MessageCallback, 0);
	glEnable(GL_DEBUG_OUTPUT);
		

	//Polygon for texture
	GLfloat Verts[] =
	{
		-1.0f,-1.0f,0.0f,	0.0f,0.0f, //Botom Left
		1.0f,-1.0f,0.0f,	1.0f,0.0f, //Botom right
		-1.0f,1.0f,0.0f,	0.0f,1.0f, //Top Left
		1.0f,1.0f,0.0f,		1.0f,1.0f, //Top Left

	};

	GLuint Inds[] =
	{
		0,1,2,
		2,1,3
	};

	//mesh.CreateMesh(Verts, Inds, 20, 6);
	
	
	shader.CreateFromFiles("Shaders/Shader.vert", "Shaders/Shader.frag");

	Texture.CreateTexture(TexWidth,TexHeight);
	Texture.UseTexture();

	//SplineOneAttempt();
	//SplineTwoAttempt();
	
	//attempt3
	//Make array
	int NumberOfPoints = 100;
	float t = 1.0f / NumberOfPoints;

	Posa.x = -0.8f;
	Posa.y = 0.80f;
	Posd.x = 0.8f;
	Posd.y = -0.80f;

	Points.push_back(0.0f);
	Points.push_back(0.0f);
	Points.push_back(0.0f);


	if (RemakeBuffer)
	{

		for (size_t i = 0; i < NumberOfPoints; i++)
		{
			Points.push_back(BezForShader(t * i, Posa.x, Posb.x, Posc.x, Posd.x));
			Points.push_back(BezForShader(t * i, Posa.y, Posb.y, Posc.y, Posd.y));
			Points.push_back(0.0f);
		}
		RemakeBuffer = false;
	}

	//glEnable(GL_DEBUG_OUTPUT);

	//Create buffers for lines (Make them dynamic)
	GLuint LineBuffer,VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &LineBuffer);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, LineBuffer);
	glBufferData(GL_ARRAY_BUFFER, Points.size() * sizeof(GLfloat), Points.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); //Changed to sizeof GLfloat because it makes more sense.
	glEnableVertexAttribArray(0);

	GLuint Square, VAOB;
	glGenVertexArrays(1, &VAOB);
	glGenBuffers(1, &Square);
	glBindVertexArray(VAOB);
	glBindBuffer(GL_ARRAY_BUFFER, Square);
	glBufferData(GL_ARRAY_BUFFER, Points.size() * sizeof(GLfloat), Points.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); //Changed to sizeof GLfloat because it makes more sense.
	glEnableVertexAttribArray(0);

	shader.ClearShader();
	shader.CreateFromFiles("Shaders/Basic2DShader.vert", "Shaders/Basic2DShader.frag");
	glEnable(GL_LINE_SMOOTH);

	glLineWidth(10.0f);

	//glEnable(GL_POINTS);
	glPointSize(10.0f);
	
	while (!window.GetWindowShouldClose())
	{
		if (RemakeBuffer)
		{
			Remake(t,LineBuffer);
		}
		PlayerInput();
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.01f, 0.1f, 0.02f, 1.0f);

		shader.UseShader();
		glBindVertexArray(VAO);
		
		//glDrawArrays(GL_POINTS, 0, 350);
		glDrawArrays(GL_LINES, 0, NumberOfPoints * 3);

		glBindVertexArray(VAOB);
		glDrawArrays(GL_LINE_LOOP, 0, 4);

		window.SwapBuffers();
		
	}
	glDisable(GL_DEBUG_OUTPUT);
	//draw lines
}

void Remake(float t,GLuint BufferIn)
{
	if (RemakeBuffer)
	{
		Points.clear();
		for (size_t i = 0; i < 10; i++)
		{
			Points.push_back(BezForShader(t * i, Posa.x, Posb.x, Posc.x, Posd.x));
			Points.push_back(BezForShader(t * i, Posa.y, Posb.y, Posc.y, Posd.y));
			Points.push_back(0.0f);

			Points.push_back(BezForShader(t * i, Posa.x, Posb.x, Posc.x, Posd.x));
			Points.push_back(BezForShader(t * i, Posa.y, Posb.y, Posc.y, Posd.y));
			Points.push_back(0.0f);
		}
		RemakeBuffer = false;
		glBindBuffer(GL_ARRAY_BUFFER, BufferIn);
		glBufferData(GL_ARRAY_BUFFER, Points.size() * sizeof(GLfloat), Points.data(), GL_DYNAMIC_DRAW);
	}
}

float BezForShader(float t, float wa, float wb, float wc, float wd)
{
		float tt = t * t;
		float ttt = tt * t;
		float mt = 1.0 - t;
		float mtt = mt * mt;
		float mttt = mtt * mt;

		return wa * mttt + 3 * wb * mtt * t + 3 * wc * mt * tt + wd * ttt;
}

void SplineOneAttempt()
{
	compShader.CreateComputeShaderFromFile("Shaders/Spline1.gl");
	compShader.UseShader();
	compShader.SetUniform3F("PosA", Posa.x,Posa.y, 0.0f);
	compShader.SetUniform3F("PosD", Posd.x, Posd.y, 0.0f);
	glDispatchCompute(TexWidth, TexHeight, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	Posa = glm::vec2(-0.5f, 0.50f);
	Posb = glm::vec2(0.5f, -0.50f);


	while (!window.GetWindowShouldClose())
	{
		PlayerInput();
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.01f, 0.1f, 0.02f, 1.0f);

		compShader.UseShader();
		compShader.SetUniform3F("PosB", Posa.x, Posa.y, 0.0f);
		compShader.SetUniform3F("PosC", Posb.x, Posb.y, 0.0f);
		glDispatchCompute(TexWidth, TexHeight, 1);

		shader.UseShader();

		mesh.RenderMesh();

		window.SwapBuffers();
	}
}

void SplineTwoAttempt()
{
	compShader.CreateComputeShaderFromFile("Shaders/Spline2.gl");
	compShader.UseShader();
	compShader.SetUniform3F("PosA", Posa.x,Posa.y, 0.0f);
	compShader.SetUniform3F("PosD", Posb.x, Posb.y, 0.0f);
	int LineSegments = 10;
	compShader.setUniform1F("T", 1.0f / float(LineSegments));
	compShader.setUniform1F("Distance", 1.6f / float(LineSegments));

	//Debugging
	float i = 1.0f / LineSegments;
	while (i < 1.6f)
	{
		std::cout << BezForShader(i, -0.8f, -0.5f, 0.5f, 0.8f) * 100 << " Is X\n";
		i += 1.0f / LineSegments;
	}

	//debugging end


	glDispatchCompute(LineSegments, 1, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	Posb = glm::vec2(-0.5f, 0.50f);
	Posc = glm::vec2(0.5f, -0.50f);

	while (!window.GetWindowShouldClose())
	{
		PlayerInput();
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.01f, 0.1f, 0.02f, 1.0f);

		compShader.UseShader();
		compShader.SetUniform3F("PosB", Posb.x, Posb.y, 0.0f);
		compShader.SetUniform3F("PosC", Posc.x, Posc.y, 0.0f);
		glDispatchCompute(LineSegments, 1, 1);

		shader.UseShader();

		mesh.RenderMesh();

		window.SwapBuffers();
	}
}

void PlayerInput()
{
	float Power = .510f;

	if (window.GetKeyIDArray()[GLFW_KEY_W])
	{
		Posc.y += 0.1 * Power;
		RemakeBuffer = true;
	}
	if (window.GetKeyIDArray()[GLFW_KEY_S])
	{
		Posc.y -= 0.1 * Power;
		RemakeBuffer = true;
	}

	if (window.GetKeyIDArray()[GLFW_KEY_D])
	{
		Posc.x += 0.1 * Power;
		RemakeBuffer = true;
	}
	if (window.GetKeyIDArray()[GLFW_KEY_A])
	{
		Posc.x -= 0.1 * Power;
		RemakeBuffer = true;
	}


	if (window.GetKeyIDArray()[GLFW_KEY_UP])
	{
		Posb.y += 0.1 * Power;
		RemakeBuffer = true;
	}
	if (window.GetKeyIDArray()[GLFW_KEY_DOWN])
	{
		Posb.y -= 0.1 * Power;
		RemakeBuffer = true;
	}

	if (window.GetKeyIDArray()[GLFW_KEY_RIGHT])
	{
		Posb.x += 0.1 * Power;
		RemakeBuffer = true;
	}
	if (window.GetKeyIDArray()[GLFW_KEY_LEFT])
	{
		Posb.x -= 0.1 * Power;
		RemakeBuffer = true;
	}



}
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	//main stuff we want to know.
	std::cout << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "**ERROR**" : "") << ". Message = " << message << std::endl;
	//additional information included in the message.
	std::cout << "(Additional information) \ntype = 0x" << type << ". severity = 0x" << severity << ".\n\n";

}