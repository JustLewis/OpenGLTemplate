#pragma once

#include <stdio.h>
#include <fstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
class Texture
{
public:
	Texture();
	Texture(const char* FileLocationIn);

	bool LoadTexture(GLfloat* TexData);

	void UseTexture();
	void ClearTexture();

	~Texture();
private:

	GLuint TextureID;

	int Width, Height, NRComponents; //Not sure what NRComponents stands for, but it seems related to the RBG, RBGA colour info

	glm::mat2 UniformSampler;

	const char* FileLocation;

};

Texture::Texture()
{
	TextureID = 0;
	Width = 0;
	Height = 0;
	NRComponents = 0;
	FileLocation = "";
}

Texture::Texture(const char* FileLocationIn)
{

	TextureID = 0;
	Width = 0;
	Height = 0;
	NRComponents = 0;
	FileLocation = FileLocationIn;
}

bool Texture::LoadTexture(GLfloat* TexData)
{
	//Learned from https://learnopengl.com/Advanced-OpenGL/Blending
	//And Course.
	//unsigned char* TexData = stbi_load(FileLocation, &Width, &Height, &NRComponents, 0);
	if (TexData) {
		printf("TexData is %u", TexData);
		GLenum Format = 0;
		if (NRComponents == 1)
		{
			Format = GL_RED;
		}
		else if (NRComponents == 3)
		{
			Format = GL_RGB;
		}
		else if (NRComponents == 4)
		{
			Format = GL_RGBA;
		}
		else
		{
			printf("NRComponents format is %i and is not setting correct format in LoadTexture.\n", NRComponents);
		}

		GLenum TextureType = GL_TEXTURE_2D;

		glGenTextures(1, &TextureID);
		glBindTexture(TextureType, TextureID);


		//how does this format bit work?
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(TextureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(TextureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(TextureType);
		glTexImage2D(TextureType, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexData);

		glBindTexture(GL_TEXTURE_2D, 0);
		//stbi_image_free(TexData);
		return true;
	}
	else {
		printf("Failed to load texture path: %s\n", FileLocation);
		///stbi_image_free(TexData);
		return false;
	}
	printf("Texture.cpp... Really shouldn't have made it this far");
	return false;
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &TextureID);
	TextureID = 0;
	Width = 0;
	Height = 0;
	NRComponents = 0;
	FileLocation = 0;
}

Texture::~Texture()
{
	ClearTexture();
}
