#version 430
layout(local_size_x = 1) in;

layout(binding = 0) buffer Pos {
    vec2 Position[];
};
layout(binding = 1) buffer Vel {
    vec2 Velocity[];
};

uniform vec2 MouseDirection;
uniform vec2 MousePosition;
uniform float DeltaT = 0.0025;
uniform float MaxDist = 1;

void main()
{
    uint ID = gl_GlobalInvocationID.x;
    vec2 ParticlePos = Position[ID];

    vec2 Direction = MousePosition - ParticlePos;
    vec2 DirectionalForce = (1000 / length(Direction)) * normalize(Direction); //if close enough, will pull towards mouse, if not no force.
    if (length(Direction) > MaxDist)
    {
        // do nothing
        Position[ID] = vec2(ParticlePos + Velocity[ID] * DeltaT * DeltaT);
    }
    vec2 appliedforce = DirectionalForce * 0.1;
    Position[ID] = vec2(ParticlePos + Velocity[ID] * DeltaT + 0.5 * appliedforce * DeltaT * DeltaT); //Basic euler acceleration
    Velocity[ID] += vec2(Velocity[ID] * appliedforce * DeltaT);
}