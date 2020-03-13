#pragma once
#include <stdio.h>
#include <tgmath.h>
#include <vector>
#include <glad/glad.h>

class Mesh
{
public:
	Mesh();
	~Mesh() { ClearMesh(); };

	void ClearMesh();
	virtual void CreateMesh(GLfloat* Vertices, GLuint* Indices, GLuint NumberOfVerts, GLuint NumberOfIndicesIn);
	void CreateProceduralSphere(GLfloat RadiusIn, GLuint HfacesIn, GLuint VfacesIn);
	void RecreateSphere(GLfloat RadiusIn, GLuint HFacesIn, GLuint VFacesIn);

	void CreateTriangle();

	virtual void RenderMesh();
	virtual void RenderPoints();

protected:
	GLuint VAO, VBO, IBO;
	GLuint NumberOfIndices;

private:
	GLfloat ToRadians(float ValueIn);
	std::vector<GLfloat> Verts;
	std::vector<GLuint> Inds;
};

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	NumberOfIndices = 0;
}

void Mesh::CreateTriangle()
{
	GLfloat Verts[] =
	{
		-0.5f,0.0f,0.0f,0.0f,0.0f,
		0.5f,0.0f,0.0f,1.0f,0.0f,
		0.0f,0.5f,0.0f,0.5f,1.0f
	};

	GLuint Inds[] =
	{
		0,1,2
	};

	CreateMesh(Verts, Inds, 15, 3);
}

void Mesh::ClearMesh()
{
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	Verts.clear();
	Inds.clear();
}

void Mesh::CreateMesh(GLfloat* Vertices, GLuint* Indices, GLuint NumberOfVerts, GLuint NumberOfIndicesIn)
{

	NumberOfIndices = NumberOfIndicesIn;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * NumberOfVerts, Vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * NumberOfIndices, Indices, GL_STATIC_DRAW);


	/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]) * 6, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]) * 6, (void*)(sizeof(Vertices[0]) * 3));
	glEnableVertexAttribArray(1);*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]) * 5, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]) * 5, (void*)(sizeof(Vertices[0]) * 3));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glDisableVertexAttribArray(0); //Does literally nothing it seems.
	//glDisableVertexAttribArray(1); //Does literally nothing it seems.

}

void Mesh::CreateProceduralSphere(GLfloat RadiusIn, GLuint HfacesIn, GLuint VfacesIn)
{

	const int TotalFaces = HfacesIn * VfacesIn;
	//std::vector<GLfloat> Verts;
	Verts.resize(TotalFaces * 5 * 3); //4 corners to a face, 5 pieces of vertex data - 3 for xyz positions, 2 for xy tex coords 

	//std::vector<GLuint> Inds;
	Inds.resize(TotalFaces * 6); //6 values per index to make a quad, half of those needed for whole sphere.

	GLfloat Radius = 0.7f;
	unsigned int VerticesCount = 0;

	//Calculate verts
	for (GLfloat i = 0; i <= 180; i += (360 / VfacesIn))
	{
		printf("%f\n", i);
		float theta = ToRadians(i); //get current angle
		float CurrentX = Radius * sinf(theta);
		float CurrentY = Radius * cosf(theta);

		for (GLfloat j = 0; j <= 359; j += (360 / HfacesIn))
		{
			//Vertices
			float theta = ToRadians(j); //get current angle
			Verts[VerticesCount] = CurrentX * sinf(theta);
			Verts[VerticesCount + 1] = CurrentY;
			Verts[VerticesCount + 2] = CurrentX * cosf(theta);

			//texture coords
			Verts[VerticesCount + 3] = 0.0f;
			Verts[VerticesCount + 4] = 0.0f; //TODO, calculate these texture coords

			//Debug
			printf("Vert %i  ", VerticesCount / 5);
			printf("X: %f	", Verts[VerticesCount]);
			printf("Y: %f	", Verts[VerticesCount + 1]);
			printf("Z: %f\n", Verts[VerticesCount + 2]);
			VerticesCount += 5;

		}
		printf("\n");
	}
	printf("\nVertices count: %i. Number of floats = %i\n\n", VerticesCount / 3, VerticesCount);


	unsigned int EdgeCheckCount = 0;
	//calculate inds
	for (GLuint j = 0; j < (TotalFaces * 6); j += 6)
	{

		bool EdgeCheck = false;
		if (EdgeCheckCount % HfacesIn == HfacesIn - 1) { EdgeCheck = true; } //To see if we've reach the edge of the line //Using modulo to check the remained = the numvber of Hfaces - 1

		//first triangle
		Inds[j] = (EdgeCheckCount);
		Inds[j + 1] = (EdgeCheckCount + HfacesIn);
		Inds[j + 2] = (EdgeCheckCount + HfacesIn + 1);

		//second triangle
		Inds[j + 3] = (EdgeCheckCount + HfacesIn + 1);
		Inds[j + 4] = (EdgeCheckCount + 1);
		Inds[j + 5] = (EdgeCheckCount);

		if (EdgeCheck)
		{
			//update these indices if at edge
			Inds[j + 2] = EdgeCheckCount + 1;
			Inds[j + 3] = EdgeCheckCount + 1;
			Inds[j + 4] = EdgeCheckCount + 1 - HfacesIn;
		}
		//debug.. THis may be in the wrong place to check as still in loop idiot...
		printf("%i, %i, %i,   %i,%i,%i\n", Inds[j], Inds[j + 1], Inds[j + 2], Inds[j + 3], Inds[j + 4], Inds[j + 5]);
		EdgeCheckCount++;
	}

	CreateMesh(Verts.data(), Inds.data(), TotalFaces * 4 * 5, TotalFaces * 6);
}

void Mesh::RecreateSphere(GLfloat RadiusIn, GLuint HFacesIn, GLuint VFacesIn)
{
	ClearMesh();
	CreateProceduralSphere(RadiusIn, HFacesIn, VFacesIn);
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, NumberOfIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Mesh::RenderPoints()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS,0, NumberOfIndices * 3);
	glBindVertexArray(0);
}

GLfloat Mesh::ToRadians(float ValueIn)
{
	return ValueIn * (3.14159265359f / 180.0f);
}