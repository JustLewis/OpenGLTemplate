#pragma once
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <random>

class Particles
{
public:

	virtual void CreateParticles(GLuint NumberOfParticlesIn);

	virtual void Render();

protected:

	GLuint VertexArrayObject;
	GLuint ParticlePositionBufferObject;
	GLuint NumberOfParticles;
	
};


void Particles::CreateParticles(GLuint NumberOfParticlesIn)
{

	if (NumberOfParticlesIn < 1) 
	{ 
	NumberOfParticles = 1;
	std::cout << "\nLewis thinks you've made an ERROR \n Number of particles cannot be less than 1. Setting number of particles to 1.\n"; 
	}
	else
	{
		NumberOfParticles = NumberOfParticlesIn;
	}
	std::vector<GLfloat> ParticlePositions;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(-(int)NumberOfParticles, NumberOfParticles);
	GLfloat minimiser = 2.0f / NumberOfParticles;

	for (size_t i = 0; i < NumberOfParticles; i++)
	{
		ParticlePositions.push_back(-1 + i * minimiser); //x pos
		ParticlePositions.push_back((GLfloat)distribution(generator) * minimiser / 2.0f); //y pos

	}

	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);

	glGenBuffers(1, &ParticlePositionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, ParticlePositionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, ParticlePositions.size() * sizeof(GL_FLOAT), ParticlePositions.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	///REMOVED this. (Additional buffers for other things.)

	/*glGenBuffers(1, &ParticleTimeOffsetBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, ParticleTimeOffsetBufferObject);
	glBufferData(GL_ARRAY_BUFFER, ParticleTimeOffset.size() * sizeof(GL_FLOAT), ParticleTimeOffset.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ParticleXOffsetBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, ParticleXOffsetBufferObject);
	glBufferData(GL_ARRAY_BUFFER, ParticleXOffset.size() * sizeof(GL_FLOAT), ParticleXOffset.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Particles::Render()
{
	glBindVertexArray(VertexArrayObject);
	glDrawArrays(GL_POINTS, 0, NumberOfParticles);
	glBindVertexArray(0);
}
