#include <glad/glad.h>
#include "../Template/Particles.h"


class ComputeParticles : public Particles
{

public:
	void Render() override;
	void CreateParticles(GLuint NumberOfParticlesIn) override;
	void Dispatch();

private:
	GLuint ParticleDirectionsBuffer;
};

void ComputeParticles::Render()
{
	glBindVertexArray(VertexArrayObject);
	glDrawArrays(GL_POINTS, 0, NumberOfParticles);
	glBindVertexArray(0);
}

void ComputeParticles::Dispatch()
{
	glDispatchCompute(NumberOfParticles, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}


void ComputeParticles::CreateParticles(GLuint NumberOfParticlesIn)
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
	std::vector<GLfloat> ParticleDirections;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(-(int)NumberOfParticles, NumberOfParticles);
	GLfloat minimiser = 2.0f / NumberOfParticles;

	GLfloat ParticleWave = (3.14159265359 / 2) / NumberOfParticles;

	for (size_t i = 0; i < NumberOfParticles; i++)
	{
		ParticlePositions.push_back(-1 + i * minimiser); //x pos
		ParticlePositions.push_back((GLfloat)distribution(generator) * minimiser / 2.0f); //y pos
		//ParticlePositions.push_back(cos((GLfloat)i * minimiser * 3) + (GLfloat)distribution(generator) * minimiser / 20.0f); //y pos
		ParticleDirections.push_back(0.0f);
		ParticleDirections.push_back(0.0f);

	}

	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);

	glGenBuffers(1, &ParticlePositionBufferObject);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0,ParticlePositionBufferObject);
	glBufferData(GL_SHADER_STORAGE_BUFFER, ParticlePositions.size() * sizeof(GL_FLOAT), ParticlePositions.data(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &ParticleDirectionsBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1,ParticleDirectionsBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, ParticleDirections.size() * sizeof(GL_FLOAT), ParticleDirections.data(), GL_DYNAMIC_COPY);
	
	glBindBuffer(GL_ARRAY_BUFFER, ParticlePositionBufferObject);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

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

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindVertexArray(0);
}