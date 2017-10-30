#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TexCoords;
layout (location = 2) in vec3 in_Normal;

out vec2 ex_TexCoords;
out vec3 ex_Normal;

uniform mat4 Matrix;

uniform Camera
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

void main(void)
{
    gl_Position = ProjectionMatrix * ViewMatrix * Matrix * vec4(in_Position,1);
    ex_TexCoords = in_TexCoords;
    ex_Normal = in_Normal;        

}
