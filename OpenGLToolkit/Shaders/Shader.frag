#version 460 core
out vec4 FragColour;

uniform sampler2D Tex;
in vec2 TexCoords;

void main()
{
	
	
	FragColour = texture(Tex,TexCoords);

}