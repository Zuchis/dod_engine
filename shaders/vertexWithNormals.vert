#version 330 core

in vec3 in_Position;
in vec2 in_TexCoords;
in vec3 in_Normal;

out vec2 ex_TexCoords;
out vec3 ex_Normal;

uniform mat4 Matrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void)
{
    gl_Position = ProjectionMatrix * ViewMatrix * Matrix * vec4(in_Position,1);
    ex_TexCoords = in_TexCoords;
    ex_Normal = in_Normal;        

}
