#include "ComputeTexture.h"

ComputeTexture::ComputeTexture()
{
	Tex = -1;
	Width = 0;
	Height = 0;
}

void ComputeTexture::CreateTexture(GLuint WidthIn, GLuint HeightIn)
{
	Width = WidthIn;
	Height = HeightIn;
	glGenTextures(1, &Tex);
	glBindTexture(GL_TEXTURE_2D, Tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE,nullptr);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, Width, Height);
	glBindImageTexture(0, Tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
	
	glGenerateMipmap(GL_TEXTURE_2D);

}
