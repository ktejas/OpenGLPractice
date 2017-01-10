#version 330 core

layout(location = 0) in vec4 Position;
uniform float gScale;
uniform mat4 gWorld;

void main()
{
	gl_Position = gWorld * vec4(gScale * Position.x, gScale * Position.y, Position.z, 1.0f);
}