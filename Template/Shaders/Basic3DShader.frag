#version 330 core
out vec4 FragColour;

uniform vec4 ObjColour;

void main()
{
	//FragColour = vec4(1.0f,0.5f,0.2f,1.0f);
	FragColour = ObjColour;
}
