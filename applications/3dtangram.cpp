#include "../shaderprogram.h"
#include "camera.h"

#define CAPTION "Badass Squirrel"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;
GLuint VaoId[3], VboId[2];
GLuint squareVboId[2];
GLuint paralelVboId[2];

GLfloat lastX = (GLfloat)(WinX / 2);
GLfloat lastY = (GLfloat)(WinY / 2);

ShaderProgram program = ShaderProgram();

Camera camera = Camera();

bool* keyStates = new bool[256];
bool firstTimeMouse = true;
bool leftMouseButtonPressed = false;
bool perspectiveMode = true;

GLfloat deltaTime = 0.0f;
GLfloat oldTime = 0.0f;

/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if(isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs

void createShaderProgram()
{
        program.compileShaderFromFile("shaders/3dvertexShader.vert",ShaderType::VERTEX);

        program.compileShaderFromFile("shaders/3dfragmentShader.frag",ShaderType::FRAGMENT);

        program.bindAttribLocation(VERTICES,"in_Position");
        program.bindAttribLocation(COLORS,"in_Color");

        program.link();

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	glUseProgram(0);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}

const GLfloat triangle[] = 
{
        // X    Y      Z     W       R     G       B      A
    // FRONT
        0.00f, 0.00f, 1.0f, 1.0f , 1.00f, 0.00f, 0.00f, 1.00f, // 0 
        0.25f, 0.00f, 1.0f, 1.0f , 1.00f, 0.00f, 0.00f, 1.00f, // 1
        0.25f, 0.25f, 1.0f, 1.0f , 1.00f, 0.00f, 0.00f, 1.00f, // 2

    // BACK
        0.00f, 0.00f, 0.0f, 1.0f , 0.70f, 0.01f, 0.02f, 1.00f, // 3
        0.25f, 0.25f, 0.0f, 1.0f , 0.70f, 0.01f, 0.02f, 1.00f, // 5
        0.25f, 0.00f, 0.0f, 1.0f , 0.70f, 0.01f, 0.02f, 1.00f, // 4

    // BOTTOM
        0.00f, 0.00f, 1.0f, 1.0f , 0.43f, 0.03f, 0.03f, 1.00f, // 0 
        0.00f, 0.00f, 0.0f, 1.0f , 0.43f, 0.03f, 0.03f, 1.00f, // 3
        0.25f, 0.00f, 0.0f, 1.0f , 0.43f, 0.03f, 0.03f, 1.00f, // 4
        0.25f, 0.00f, 0.0f, 1.0f , 0.43f, 0.03f, 0.03f, 1.00f, // 4
        0.25f, 0.00f, 1.0f, 1.0f , 0.43f, 0.03f, 0.03f, 1.00f, // 1
        0.00f, 0.00f, 1.0f, 1.0f , 0.43f, 0.03f, 0.03f, 1.00f, // 0 

    // RIGHT
        0.25f, 0.00f, 1.0f, 1.0f , 1.00f, 0.20f, 0.30f, 1.00f, // 1
        0.25f, 0.00f, 0.0f, 1.0f , 1.00f, 0.20f, 0.30f, 1.00f, // 4
        0.25f, 0.25f, 0.0f, 1.0f , 1.00f, 0.20f, 0.30f, 1.00f, // 5
        0.25f, 0.25f, 0.0f, 1.0f , 1.00f, 0.20f, 0.30f, 1.00f, // 5
        0.25f, 0.25f, 1.0f, 1.0f , 1.00f, 0.20f, 0.30f, 1.00f, // 2
        0.25f, 0.00f, 1.0f, 1.0f , 1.00f, 0.20f, 0.30f, 1.00f, // 1

    // LEFT
        0.00f, 0.00f, 1.0f, 1.0f , 0.84f, 0.15f, 0.16f, 1.00f, // 0 
        0.25f, 0.25f, 1.0f, 1.0f , 0.84f, 0.15f, 0.16f, 1.00f, // 2
        0.25f, 0.25f, 0.0f, 1.0f , 0.84f, 0.15f, 0.16f, 1.00f, // 5
        0.25f, 0.25f, 0.0f, 1.0f , 0.84f, 0.15f, 0.16f, 1.00f, // 5
        0.00f, 0.00f, 0.0f, 1.0f , 0.84f, 0.15f, 0.16f, 1.00f, // 3
        0.00f, 0.00f, 1.0f, 1.0f , 0.84f, 0.15f, 0.16f, 1.00f, // 0 
};

const GLfloat square[] =
{
    // FRONT
        0.00f, 0.00f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 0
        0.25f, 0.00f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 1
        0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 2
        0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 2
        0.00f, 0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 3
        0.00f, 0.00f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 0

    // RIGHT
        0.25f, 0.00f, 1.0f, 1.0f, 0.1f, 0.9f, 0.1f, 1.0f, // 1
        0.25f, 0.00f, 0.0f, 1.0f, 0.1f, 0.9f, 0.1f, 1.0f, // 5
        0.25f, 0.25f, 0.0f, 1.0f, 0.1f, 0.9f, 0.1f, 1.0f, // 6
        0.25f, 0.25f, 0.0f, 1.0f, 0.1f, 0.9f, 0.1f, 1.0f, // 6
        0.25f, 0.25f, 1.0f, 1.0f, 0.1f, 0.9f, 0.1f, 1.0f, // 2
        0.25f, 0.00f, 1.0f, 1.0f, 0.1f, 0.9f, 0.1f, 1.0f, // 1

    // TOP
        0.25f, 0.25f, 1.0f, 1.0f, 0.25f, 0.75f, 0.25f, 1.0f, // 2
        0.25f, 0.25f, 0.0f, 1.0f, 0.25f, 0.75f, 0.25f, 1.0f, // 6
        0.00f, 0.25f, 0.0f, 1.0f, 0.25f, 0.75f, 0.25f, 1.0f, // 7
        0.00f, 0.25f, 0.0f, 1.0f, 0.25f, 0.75f, 0.25f, 1.0f, // 7
        0.00f, 0.25f, 1.0f, 1.0f, 0.25f, 0.75f, 0.25f, 1.0f, // 3
        0.25f, 0.25f, 1.0f, 1.0f, 0.25f, 0.75f, 0.25f, 1.0f, // 2

    // BACK
        0.25f, 0.00f, 0.0f, 1.0f, 0.35f, 0.60f, 0.35f, 1.0f, // 5
        0.00f, 0.00f, 0.0f, 1.0f, 0.35f, 0.60f, 0.35f, 1.0f, // 4
        0.00f, 0.25f, 0.0f, 1.0f, 0.35f, 0.60f, 0.35f, 1.0f, // 7
        0.00f, 0.25f, 0.0f, 1.0f, 0.35f, 0.60f, 0.35f, 1.0f, // 7
        0.25f, 0.25f, 0.0f, 1.0f, 0.35f, 0.60f, 0.35f, 1.0f, // 6
        0.25f, 0.00f, 0.0f, 1.0f, 0.35f, 0.60f, 0.35f, 1.0f, // 5

    // LEFT
        0.00f, 0.00f, 0.0f, 1.0f, 0.52f, 0.84f, 0.27f, 1.0f, // 4
        0.00f, 0.00f, 1.0f, 1.0f, 0.52f, 0.84f, 0.27f, 1.0f, // 0
        0.00f, 0.25f, 1.0f, 1.0f, 0.52f, 0.84f, 0.27f, 1.0f, // 3
        0.00f, 0.25f, 1.0f, 1.0f, 0.52f, 0.84f, 0.27f, 1.0f, // 3
        0.00f, 0.25f, 0.0f, 1.0f, 0.52f, 0.84f, 0.27f, 1.0f, // 7
        0.00f, 0.00f, 0.0f, 1.0f, 0.52f, 0.84f, 0.27f, 1.0f, // 4

    // BOTTOM
        0.00f, 0.00f, 1.0f, 1.0f, 0.0f, 0.47f, 0.10f, 1.0f, // 0
        0.00f, 0.00f, 0.0f, 1.0f, 0.0f, 0.47f, 0.10f, 1.0f, // 4
        0.25f, 0.00f, 0.0f, 1.0f, 0.0f, 0.47f, 0.10f, 1.0f, // 5
        0.25f, 0.00f, 0.0f, 1.0f, 0.0f, 0.47f, 0.10f, 1.0f, // 5
        0.25f, 0.00f, 1.0f, 1.0f, 0.0f, 0.47f, 0.10f, 1.0f, // 1
        0.00f, 0.00f, 1.0f, 1.0f, 0.0f, 0.47f, 0.10f, 1.0f, // 0

};

const GLfloat paralel[] =
{
    // FRONT
    0.25f, 0.00f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 0
    0.50f, 0.00f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 1
    0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 2

    0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 2
    0.00f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 3
    0.25f, 0.00f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 0
    0.00f, 0.00f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 5
    0.25f, 0.00f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 0
    0.00f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 3

    0.00f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 3
   -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 4
    0.00f, 0.00f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 5

    // BACK
    0.25f, 0.25f, 0.0f, 1.0f, 0.04f, 0.02f, 0.4f, 1.0f, // 8
    0.50f, 0.00f, 0.0f, 1.0f, 0.04f, 0.02f, 0.4f, 1.0f, // 7
    0.25f, 0.00f, 0.0f, 1.0f, 0.04f, 0.02f, 0.4f, 1.0f, // 6
    
    0.25f, 0.00f, 0.0f, 1.0f, 0.04f, 0.02f, 0.4f, 1.0f, // 6
    0.00f, 0.00f, 0.0f, 1.0f, 0.04f, 0.02f, 0.4f, 1.0f, // 11
    0.25f, 0.25f, 0.0f, 1.0f, 0.04f, 0.02f, 0.4f, 1.0f, // 8
    0.00f, 0.00f, 0.0f, 1.0f, 0.04f, 0.02f, 0.4f, 1.0f, // 11
    0.00f, 0.25f, 0.0f, 1.0f, 0.04f, 0.02f, 0.4f, 1.0f, // 9
    0.25f, 0.25f, 0.0f, 1.0f, 0.04f, 0.02f, 0.4f, 1.0f, // 8

    0.00f, 0.00f, 0.0f, 1.0f, 0.04f, 0.02f, 0.4f, 1.0f, // 11
   -0.25f, 0.25f, 0.0f, 1.0f, 0.04f, 0.02f, 0.4f, 1.0f, // 10
    0.00f, 0.25f, 0.0f, 1.0f, 0.04f, 0.02f, 0.4f, 1.0f, // 9

    // TOP
    0.50f, 0.00f, 1.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 1
    0.50f, 0.00f, 0.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 7
    0.25f, 0.25f, 0.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 8
    0.25f, 0.25f, 0.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 8
    0.25f, 0.25f, 1.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 2
    0.50f, 0.00f, 1.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 1

    0.00f, 0.25f, 1.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 3
    0.25f, 0.25f, 1.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 2
    0.25f, 0.25f, 0.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 8
    0.25f, 0.25f, 0.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 8
    0.00f, 0.25f, 0.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 9
    0.00f, 0.25f, 1.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 3

   -0.25f, 0.25f, 1.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 4
    0.00f, 0.25f, 1.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 3
    0.00f, 0.25f, 0.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 9
    0.00f, 0.25f, 0.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 9
   -0.25f, 0.25f, 0.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 10
   -0.25f, 0.25f, 1.0f, 1.0f, 0.14f, 0.31f, 0.73f, 1.0f, // 4

   // BOTTOM

    0.25f, 0.00f, 1.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 0
    0.25f, 0.00f, 0.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 6
    0.50f, 0.00f, 0.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 7
    0.50f, 0.00f, 0.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 7
    0.50f, 0.00f, 1.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 1
    0.25f, 0.00f, 1.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 0

    0.25f, 0.00f, 1.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 0
    0.00f, 0.00f, 1.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 5
    0.00f, 0.00f, 0.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 11
    0.00f, 0.00f, 0.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 11
    0.25f, 0.00f, 0.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 6
    0.25f, 0.00f, 1.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 0

    0.00f, 0.00f, 0.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 11
    0.00f, 0.00f, 1.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 5
   -0.25f, 0.25f, 1.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 4
   -0.25f, 0.25f, 1.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 4
   -0.25f, 0.25f, 0.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 10
    0.00f, 0.00f, 0.0f, 1.0f, 0.14f, 0.55f, 0.73f, 1.0f, // 11

};

void createBufferObjects()
{
    glGenVertexArrays(3, VaoId);
    glBindVertexArray(VaoId[0]);
    {
        glGenBuffers(2, VboId);

        glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);

        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

        glEnableVertexAttribArray(VERTICES);

        glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);

        glEnableVertexAttribArray(COLORS);

        glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid *)(sizeof(GLfloat) * 4));

    }
    glBindVertexArray(VaoId[1]);
    {
        glGenBuffers(2, squareVboId);

        glBindBuffer(GL_ARRAY_BUFFER, squareVboId[0]);

        glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

        glEnableVertexAttribArray(VERTICES);

        glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);

        glEnableVertexAttribArray(COLORS);

        glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid *)(sizeof(GLfloat) * 4));
    }
    glBindVertexArray(VaoId[2]);
    {
        glGenBuffers(2, paralelVboId);

        glBindBuffer(GL_ARRAY_BUFFER, paralelVboId[0]);

        glBufferData(GL_ARRAY_BUFFER, sizeof(paralel), paralel, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(VERTICES);
        
        glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);

        glEnableVertexAttribArray(COLORS);

        glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid *)(sizeof(GLfloat) * 4));
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
    glBindVertexArray(VaoId[0]);
    glDisableVertexAttribArray(VERTICES);
    glDeleteBuffers(2, VboId);
    glDeleteVertexArrays(2, VaoId);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

// big triangles
Matrix4 _M0 = math::CreateTransformMatrix( -0.15f,-0.2f,0.0f,170.0f,2.0f);
  float* M0 = _M0.getData();

Matrix4 _M3 = math::CreateTransformMatrix( 0.35f,-0.28f,0.0,125.0f,2.0f);
  float* M3 = _M3.getData();

// medium triangle
Matrix4 _M4 = math::CreateTransformMatrix( -0.13f,0.40f,0.0,78.0f,1.5f);
  float* M4 = _M4.getData();

// small triangles
Matrix4 _M5 = math::CreateTransformMatrix( 0.15f,-0.55f,0.0,80.0f,1.2f);
  float* M5 = _M5.getData();

Matrix4 _M6 = math::CreateTransformMatrix( -0.97f,-0.2f,0.0f,-12.0f,1.2f);
  float* M6 = _M6.getData();

// square
Matrix4 _M1 = math::CreateTransformMatrix(-0.51f,0.41f,0.0f,33.0f,1.25f);
  float* M1 = _M1.getData();

 // pararelogram
Matrix4 _M2 = math::CreateTransformMatrix(-0.12f,0.4f,0.0f,-56.0f,1.0f);
  float* M2 = _M2.getData();

GLfloat ViewMatrix1[] = {
    0.70f, -0.41f,  0.58f,  0.00f,
    0.00f,  0.82f,  0.58f,  0.00f,
   -0.70f, -0.41f,  0.58f,  0.00f,
    0.00f,  0.00f, -8.70f,  1.00f
}; // Column Major

GLfloat ViewMatrix2[] = {
   -0.70f, -0.41f, -0.58f,  0.00f,
    0.00f,  0.82f, -0.58f,  0.00f,
    0.70f, -0.41f, -0.58f,  0.00f,
    0.00f,  0.00f, -8.70f,  1.00f
}; // Column Major

GLfloat ProjectionMatrix1[] = {
    0.50f,  0.00f,  0.00f,  0.00f,
    0.00f,  0.50f,  0.00f,  0.00f,
    0.00f,  0.00f, -0.22f,  0.00f,
    0.00f,  0.00f, -1.22f,  1.00f
}; // Column Major

GLfloat ProjectionMatrix2[] = {
    2.79f,  0.00f,  0.00f,  0.00f,
    0.00f,  3.73f,  0.00f,  0.00f,
    0.00f,  0.00f, -1.22f, -1.00f,
    0.00f,  0.00f, -2.22f,  0.00f
}; // Column Major

void applyMotion() {
    if(keyStates['w'])
        camera.computeKeyboardInputs(FORWARD,deltaTime);
    if(keyStates['s'])
        camera.computeKeyboardInputs(BACKWARD,deltaTime);
    if(keyStates['a'])
        camera.computeKeyboardInputs(LEFT,deltaTime);
    if(keyStates['d'])
        camera.computeKeyboardInputs(RIGHT,deltaTime);
}


Matrix4 ortho = math::Ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 10.0f);

GLfloat* projectionMatrix;

void getProjection(){
}


void drawScene()
{
    GLfloat currentTime = ((GLfloat)glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
    deltaTime = currentTime - oldTime;
    oldTime = currentTime;
    applyMotion();
    program.use();
    Matrix4 viewMatrix = camera.getViewMatrix();

    Matrix4 perspective = math::Perspective(camera.zoom, WinX / WinY, 1.0f, 10.0f);
    if(perspectiveMode)
        projectionMatrix = perspective.getData();
    else
        projectionMatrix = ortho.getData();

    program.setUniform("ViewMatrix",viewMatrix.getData());
    //program.setUniform("ViewMatrix",ViewMatrix1);
    program.setUniform("ProjectionMatrix",projectionMatrix);
    // triangles
    glBindVertexArray(VaoId[0]);

    // big triangles
    program.setUniform("Matrix",M0);
    glDrawArrays(GL_TRIANGLES, 0, 24);

    program.setUniform("Matrix",M3);
    glDrawArrays(GL_TRIANGLES, 0, 24);

    // medium triangle
    program.setUniform("Matrix",M4);
    glDrawArrays(GL_TRIANGLES, 0, 24);

    //small triangles
    
    program.setUniform("Matrix",M5);
    glDrawArrays(GL_TRIANGLES, 0, 24);

    program.setUniform("Matrix",M6);
    glDrawArrays(GL_TRIANGLES, 0, 24);

    // square
    glBindVertexArray(VaoId[1]);
    program.setUniform("Matrix",M1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
 
    // pararelogram
    glBindVertexArray(VaoId[2]);
    program.setUniform("Matrix",M2);
    glDrawArrays(GL_TRIANGLES, 0, 60);

    glUseProgram(0);
    glBindVertexArray(0);

    checkOpenGLError("ERROR: Could not draw scene.");
}

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
}

void timer(int value)
{
    std::ostringstream oss;
    oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
    std::string s = oss.str();
    glutSetWindow(WindowHandle);
    glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

void keyboardPress(unsigned char key, int x, int y) {
    if(key == 'p' || key == 'P')
        perspectiveMode = !perspectiveMode;
    keyStates[key] = true;
}

void keyboardUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void mousePress(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        //std::cout << "apertou uhull" << std::endl;
        leftMouseButtonPressed = true;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        //std::cout << "soltou" << std::endl;
        leftMouseButtonPressed = false;
        firstTimeMouse = true;

    if (button == MOUSE_SCROLL_UP){
        camera.computeMouseScroll((GLfloat)SPEED / 2.0f);
    }

    if (button == MOUSE_SCROLL_DOWN)
        camera.computeMouseScroll(-(GLfloat)SPEED / 2.0f);

}

void mouseMovement(int x, int y){
    if(leftMouseButtonPressed){
        if(firstTimeMouse){
            lastX = x;
            lastY = y;
            firstTimeMouse = false;
        }
        GLfloat xOffset = x - lastX;
        GLfloat yOffset = y - lastY;

        lastX = x;
        lastY = y;

        camera.computeMouseMovement(xOffset,-yOffset);
    }
}

//void trackMouseMovement(int x, int y){
//}



/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer,0);
        glutKeyboardFunc(keyboardPress);
        glutKeyboardUpFunc(keyboardUp);
        glutMouseFunc(mousePress);
        glutMotionFunc(mouseMovement);
}

void checkOpenGLInfo()
{
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL()
{
	checkOpenGLInfo();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() 
{
	glewExperimental = GL_TRUE;
	GLenum result = glewInit() ; 
	if (result != GLEW_OK) { 
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	} 
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if(WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	createShaderProgram();
	createBufferObjects();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
        keyStates['p'] = true;
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}
