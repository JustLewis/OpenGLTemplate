#pragma once
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
class Window
{
public:
	Window();
	Window(GLuint WindowWidthIn, GLuint WindowHeightIn);
	~Window();
	int Initialise();

	void WindowName(const char* NewName) { glfwSetWindowTitle(MainWindow,NewName); }

	GLint GetBufferWidth() { return BufferWidth; }
	GLint GetBufferHeight() { return BufferHeight; }
	
	bool* GetKeyIDArray() { return KeyPressed; }

	bool GetWindowShouldClose() { return glfwWindowShouldClose(MainWindow); }
	void SwapBuffers() { glfwSwapBuffers(MainWindow); };

	GLfloat GetMouseXChange();
	GLfloat GetMouseYChange();

private:

	GLFWwindow *MainWindow;
	GLuint WindowWidth, WindowHeight;
	GLint BufferWidth, BufferHeight;

	bool KeyPressed[1024];

	GLfloat MouseLastX, MouseLastY, MouseXChange, MouseYChange;
	bool MouseFirstMoved;

	static void HandleKeys(GLFWwindow* WindowIn, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* WindowIn, double XPos, double Ypos);
	void CreateCallbacks();

};



Window::Window()
{
	WindowWidth = 400;
	WindowHeight = 400;


	for (size_t i = 0; i < 1024; i++)
	{
		KeyPressed[i] = false;
	}


	BufferWidth = 0;
	BufferHeight = 0;
}

Window::Window(GLuint WindowWidthIn, GLuint WindowHeightIn)
{
	WindowWidth = WindowWidthIn;
	WindowHeight = WindowHeightIn;

	for (size_t i = 0; i < 1024; i++)
	{
		KeyPressed[i] = false;
	}
	BufferWidth = 0;
	BufferHeight = 0;
}

Window::~Window()
{
	glfwDestroyWindow(MainWindow);
	glfwTerminate();
}

int Window::Initialise()
{

	if (!glfwInit())
	{
		printf("Failed to initialise GLFW");
		glfwTerminate();
		return 1;
	}


	MainWindow = glfwCreateWindow(WindowWidth, WindowHeight, "OpenGL4.6", nullptr, nullptr);

	if (!MainWindow)
	{
		printf("Failed to create window");
		glfwDestroyWindow(MainWindow);
		glfwTerminate();
	}

	glfwGetFramebufferSize(MainWindow, &BufferWidth, &BufferHeight);
	glfwMakeContextCurrent(MainWindow);

	CreateCallbacks();
	glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetWindowUserPointer(MainWindow, this);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD");
		return 1;
	}

	//Followed referenced book below for this little section. 
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major); //setting version of Opengl
	glGetIntegerv(GL_MINOR_VERSION, &minor); //setting version of Opengl
	printf("GL Vendor : %s\n", vendor);
	printf("GL Renderer : %s\n", renderer);
	printf("GL Version (string) : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version : %s\n\n", glslVersion);
	/*

	Wolff, David. OpenGL 4 Shading Language Cookbook : Build High-Quality,
	Real-time 3D Graphics with OpenGL 4. 6, GLSL 4. 6 and C++17, 3rd Edition,
	Packt Publishing, Limited, 2018.
	ProQuest Ebook Central, http://ebookcentral.proquest.com/lib/portsmouth-ebooks/detail.action?docID=5532271.
Created from portsmouth-ebooks on 2019-10-14 11:16:27.
	*/

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);



	glViewport(0, 0, BufferWidth, BufferHeight);


	return 0;
}

GLfloat Window::GetMouseXChange()
{
	GLfloat change = MouseXChange;
	MouseXChange = 0;

	return change;
}

GLfloat Window::GetMouseYChange()
{
	GLfloat change = MouseYChange;
	MouseYChange = 0;

	return change;
}

void Window::HandleKeys(GLFWwindow* WindowIn, int key, int code, int action, int mode)
{
	Window* TheWindow = static_cast<Window*>(glfwGetWindowUserPointer(WindowIn));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(WindowIn, GL_TRUE);
	}

	if (key >= 0 && key <= 1024)
	{
		if (action == GLFW_PRESS)
		{
			TheWindow->KeyPressed[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			TheWindow->KeyPressed[key] = false;
		}
	}

}

void Window::HandleMouse(GLFWwindow* WindowIn, double XPos, double Ypos)
{
	Window* TheWindow = static_cast<Window*>(glfwGetWindowUserPointer(WindowIn));

	if (TheWindow->MouseFirstMoved)
	{
		TheWindow->MouseLastX = XPos;
		TheWindow->MouseLastY = Ypos;
		TheWindow->MouseFirstMoved = false;
	}

	TheWindow->MouseXChange = XPos - TheWindow->MouseLastX;
	TheWindow->MouseYChange = TheWindow->MouseLastY - Ypos;

	TheWindow->MouseLastX = XPos;
	TheWindow->MouseLastY = Ypos;

}

void Window::CreateCallbacks()
{
	glfwSetKeyCallback(MainWindow, HandleKeys);
	glfwSetCursorPosCallback(MainWindow, HandleMouse);
}
