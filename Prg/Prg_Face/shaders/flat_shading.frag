#version 410 core

in vec3 color;

layout(location = 0) out vec4 final_col;

void main()
{
	final_col = vec4(color,1.0);
}
