#version 330 core

uniform float gScale;
uniform mat4 gWorld;
layout(location = 0) in vec3 Position;

void main()
{
	//gl_Position = vec4(gScale * Position.x, gScale * Position.y, 1.0f, 1.0f);
	gl_Position = gWorld * vec4(gScale * Position, 1.0f);
}