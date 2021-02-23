#version 460 core
layout(location = 0) in vec3 VertPos;
layout(location =1) in vec2 VertTexCoords;

out vec2 TexCoords;

void main()
{

	gl_Position = vec4(VertPos,1.0);
	TexCoords = VertTexCoords;
}