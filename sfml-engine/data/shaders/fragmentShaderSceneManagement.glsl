#version 330 core

in vec2 ex_TexCoords;
in vec3 ex_Normal;

out vec4 out_Color;

void main(void)
{
    out_Color = vec4(ex_Normal,1);
}
