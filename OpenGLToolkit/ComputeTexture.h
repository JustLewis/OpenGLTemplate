#pragma once
#include <glad/glad.h>


class ComputeTexture
{
public:
	ComputeTexture();

	void CreateTexture(GLuint WidthIn, GLuint HeightIn);
	void UseTexture() { glActiveTexture(Tex); }



private:
	GLuint Tex, Width, Height;
};

