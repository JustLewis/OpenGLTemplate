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

//Points for bezier curve
glm::vec2 Posa; //Start
glm::vec2 Posb; //first control point
glm::vec2 Posc; //second control point
glm::vec2 Posd; //ENd

//texture dimensions for compute shader to render to. 
GLuint TexWidth = 400;
GLuint TexHeight = 400;

//From Kronos - Debugging function protype. (Full function below main)
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

void PlayerInput(); //updates Posb and Posc values with controls (WASD and Arrow keys)
float BezForShader(float t, float wa, float wb, float wc, float wd); //Bezier Algorythm for 2 control points 
void SplineOneAttempt(); //uses compute shader
void SplineTwoAttempt(); //uses compute shader
void Remake(float t,GLuint BufferIn); //Remake buffer containing line (Not compute shader

bool RemakeBuffer = true;

std::vector<GLfloat> Points;

int main()
{
	window = Window(1920, 1080);
	//window = Window(1920*2, 1080*2); //4k
	window.Initialise();

	//glEnable(GL_DEPTH_TEST); //no need for this working in 2D

	//Debug stuff. Uncomment the below 2 functions for debug information
	//glDebugMessageCallback(MessageCallback, 0);
	//glEnable(GL_DEBUG_OUTPUT);
		



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



	//Create buffers for lines
	GLuint LineBuffer,VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &LineBuffer);
	glBindVertexArray(VAO);

	Remake(t, LineBuffer); //filles buffer on cpu

	//pass buffer to GPU for drawing
	glBindBuffer(GL_ARRAY_BUFFER, LineBuffer);
	glBufferData(GL_ARRAY_BUFFER, Points.size() * sizeof(GLfloat), Points.data(), GL_STATIC_DRAW);

	//set vertex attributes (x and y position)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0); //Enable vertex attrivute 0

	shader.CreateFromFiles("Shaders/Basic2DShader.vert", "Shaders/Basic2DShader.frag");
	
	glEnable(GL_LINE_SMOOTH); //makes the line equally thick.
	glLineWidth(10.0f);
	
	while (!window.GetWindowShouldClose())
	{
		if (RemakeBuffer)
		{
			Remake(t,LineBuffer);
		}
		PlayerInput(); //get player input
		glfwPollEvents(); //update player input
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the frame buffer
		glClearColor(0.01f, 0.1f, 0.02f, 1.0f); //set backgroun colour

		shader.UseShader(); //use this shader
		glBindVertexArray(VAO); //bind object to draw (Lines in this case)
		glDrawArrays(GL_LINE_STRIP, 0, NumberOfPoints);


		window.SwapBuffers();
		
	}
	glDisable(GL_DEBUG_OUTPUT);
	//draw lines
}

void Remake(float t,GLuint BufferIn)
{
	if (RemakeBuffer)
	{
		Points.clear(); //clear the vector
		for (size_t i = 0; i < 100; i++) //100 points along line
		{
			Points.push_back(BezForShader(t * i, Posa.x, Posb.x, Posc.x, Posd.x));
			Points.push_back(BezForShader(t * i, Posa.y, Posb.y, Posc.y, Posd.y));
		}
		RemakeBuffer = false; //reset remake buffer
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
	//Polygon for texture (for compute shader)
	GLfloat Verts[] =
	{
		-1.0f,-1.0f,0.0f,	0.0f,0.0f, //Botom Left
		1.0f,-1.0f,0.0f,	1.0f,0.0f, //Botom right
		-1.0f,1.0f,0.0f,	0.0f,1.0f, //Top Left
		1.0f,1.0f,0.0f,		1.0f,1.0f, //Top Left

	};

	//Indices for polygon (for compute shader)
	GLuint Inds[] =
	{
		0,1,2,
		2,1,3
	};

	mesh.CreateMesh(Verts, Inds, 20, 6); //create polygon

	shader.CreateFromFiles("Shaders/Shader.vert", "Shaders/Shader.frag");

	Texture.CreateTexture(TexWidth, TexHeight);
	Texture.UseTexture();

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
	//Polygon for texture (for compute shader)
	GLfloat Verts[] =
	{
		-1.0f,-1.0f,0.0f,	0.0f,0.0f, //Botom Left
		1.0f,-1.0f,0.0f,	1.0f,0.0f, //Botom right
		-1.0f,1.0f,0.0f,	0.0f,1.0f, //Top Left
		1.0f,1.0f,0.0f,		1.0f,1.0f, //Top Left

	};

	//Indices for polygon (for compute shader)
	GLuint Inds[] =
	{
		0,1,2,
		2,1,3
	};

	mesh.CreateMesh(Verts, Inds, 20, 6); //create polygon

	shader.CreateFromFiles("Shaders/Shader.vert", "Shaders/Shader.frag");

	Texture.CreateTexture(TexWidth, TexHeight);
	Texture.UseTexture();

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