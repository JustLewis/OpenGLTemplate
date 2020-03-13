#version 330 core
out vec4 FragColour;

uniform vec4 ObjColour;

void main()
{
	FragColour = ObjColour;
}
