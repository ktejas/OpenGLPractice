#version 330

uniform float gScale;
layout (location = 0) in vec3 Position;

void main()
{
    gl_Position = vec4(gScale * Position.x, gScale * Position.y, Position.z, 1.0);
}