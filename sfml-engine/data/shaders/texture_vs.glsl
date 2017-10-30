#version 330 core

in vec3 in_Position;
in vec2 in_TexCoords;
in vec3 in_Normal;

out vec2 ex_TexCoords;
out vec3 ex_Normal;

out vec3 vertPos;

out vec3 ambientColor, diffuseColor, specColor, lightPos;

uniform mat4 Matrix;
uniform vec4 LightPosition;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;

uniform Camera
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

void main(void)
{
    gl_Position = ProjectionMatrix * ViewMatrix * Matrix * vec4(in_Position,1);

    ex_TexCoords = in_TexCoords;

    //vec4 vertPos4 = Matrix * vec4(in_Position, 1);
    vec4 vertPos4 = ViewMatrix * Matrix * vec4(in_Position, 1);

    vertPos = vec3(vertPos4) / vertPos4.w;

    ex_Normal = vec3(Matrix * vec4(in_Normal, 0.0));

    ambientColor = AmbientProduct.xyz;
    diffuseColor = DiffuseProduct.xyz;
    specColor = SpecularProduct.xyz;
    lightPos = LightPosition.xyz;
}
