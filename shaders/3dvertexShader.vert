#version 330 core

in vec4 in_Position;
in vec4 in_Color;
out vec4 ex_Color;

uniform mat4 Matrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void)
{
	gl_Position = ProjectionMatrix * ViewMatrix * Matrix * in_Position;
	ex_Color = in_Color;
}
