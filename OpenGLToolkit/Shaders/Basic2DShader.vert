#version 330 core
layout (location = 0) in vec3 aPos;

uniform float Scale = 1.0f;
uniform vec2 CamPos;
out float Colour;
void main ()
{	
	Colour = aPos.z;
	gl_Position =  vec4(aPos.xy / Scale,0.0f,1.0f);
}
