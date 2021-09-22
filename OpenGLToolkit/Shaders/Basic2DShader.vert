#version 330 core
layout (location = 0) in vec2 aPos;

uniform float Scale = 1.0f;

void main ()
{
	
	gl_Position =  vec4(aPos / Scale,0.0f,1.0f);
}
