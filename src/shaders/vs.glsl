#version 410 core

layout (location=0) in vec2 pos;
layout (location=1) in vec2 uv;

out vec2 vs_uv;

void main()
{
	gl_Position = vec4(pos, 0., 1.);
	vs_uv = uv;
}
