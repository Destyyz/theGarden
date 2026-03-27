#version 410 core

layout(location=0) in vec3 vx_pos; // Indice 0
layout(location=1) in vec3 vx_nml; // Indice 1
layout(location=3) in vec3 vx_col; // Indice 3

uniform mat4 projectionMat;
uniform mat4 modelviewMat;

out vec3 color;

void main()
{
	gl_Position = projectionMat*modelviewMat*vec4(vx_pos,1.0);

	color = vx_col;
}
