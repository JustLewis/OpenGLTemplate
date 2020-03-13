#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
class Camera
{
public:
	Camera() {};
	Camera(glm::vec3 StartPosition, glm::vec3 StartUp, GLfloat StartYaw, GLfloat StartPitch, GLfloat StartMoveSpeed, GLfloat StartTurnSpeed);

	glm::vec3 GetPosition() { return Position; }
	glm::vec3 GetDirection() { return glm::normalize(FrontPos); }
	glm::mat4 CalculateViewMatrix();

	void MouseControl(GLfloat Xchange, GLfloat Ychange);
	void KeyControl(bool* Keys, GLfloat DeltaTime);

private:
	glm::vec3 Position, WorldUp, FrontPos, UpPos, RightPos;
	GLfloat Yaw, Pitch, MovementSpeed, TurnSpeed;

	void Update();
};


Camera::Camera(glm::vec3 StartPosition, glm::vec3 StartUp, GLfloat StartYaw, GLfloat StartPitch, GLfloat StartMoveSpeed, GLfloat StartTurnSpeed)
{
	Position = StartPosition;
	FrontPos = glm::vec3(0.0f, 0.0f, 1.0f);

	WorldUp = StartUp;
	Yaw = StartYaw;
	Pitch = StartPitch;

	MovementSpeed = StartMoveSpeed;
	TurnSpeed = StartTurnSpeed;

	Update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(Position, Position + FrontPos, UpPos);
}

void Camera::MouseControl(GLfloat Xchange, GLfloat Ychange)
{
	Xchange *= TurnSpeed;
	Ychange *= TurnSpeed;

	Yaw += Xchange;
	Pitch += Ychange;

	if (Pitch > 89.0f)
	{
		Pitch = 89.0f;
	}

	if (Pitch < -89.0f)
	{
		Pitch = -89.0f;
	}

	Update();
}

void Camera::KeyControl(bool* Keys, GLfloat DeltaTime)
{
	GLfloat Velocity = MovementSpeed * DeltaTime;
	//printf("Movement speed = %f\n", MovementSpeed);

	if (Keys[GLFW_KEY_W])
	{
		Position += FrontPos * Velocity;
	}

	if (Keys[GLFW_KEY_S])
	{
		Position -= FrontPos * Velocity;
	}

	if (Keys[GLFW_KEY_A])
	{
		Position -= RightPos * Velocity;
	}

	if (Keys[GLFW_KEY_D])
	{
		Position += RightPos * Velocity;
	}
}

void Camera::Update()
{
	//Good reference = https://learnopengl.com/Getting-started/Camera
	FrontPos.x = cos(glm::radians(Yaw))* cos(glm::radians(Pitch));
	FrontPos.y = sin(glm::radians(Pitch));
	FrontPos.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	FrontPos = glm::normalize(FrontPos);
	RightPos = glm::normalize(glm::cross(FrontPos, WorldUp));
	UpPos = glm::normalize(glm::cross(RightPos, FrontPos));

}
