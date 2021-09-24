#version 330 core
out vec4 FragColour;

uniform vec4 ObjColour;
in float Colour;
void main()
{
	vec3 MColour = vec3(0.5f);
	if(Colour > 0.1f)
	{
		MColour.r = 0.5f;
	}
	if(Colour > 0.2)
	{
		MColour = vec3(0.0f);
		MColour.b = 0.5f;
	}
	if(Colour > 0.4)
	{
		MColour = vec3(0.0f);
		MColour.g = 0.5f;
	}
	if(Colour > 0.7)
	{
		MColour = vec3(1.00f);
	}

	FragColour = vec4(MColour,1.0f);
}
