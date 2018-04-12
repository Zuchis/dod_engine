#include "../shaderprogram.h"

#define CAPTION "Badass Squirrel"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;
GLuint VaoId[3], VboId[2];
GLuint squareVboId[2];
GLuint paralelVboId[2];


ShaderProgram program = ShaderProgram();

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
        program.compileShaderFromFile("shaders/vertexShader.vert",ShaderType::VERTEX);
        program.compileShaderFromFile("shaders/fragmentShader.frag",ShaderType::FRAGMENT);
        program.bindAttribLocation(VERTICES,"in_Position");
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
         0.00f, 0.00f, 0.0f, 1.0f ,
         0.25f, 0.00f, 0.0f, 1.0f ,
         0.25f, 0.25f, 0.0f, 1.0f 
};

const GLuint triangleIndices[] =
{
	0,1,2
};

const GLfloat square[] =
{
    0.00f, 0.00f, 0.0f, 1.0f,
    0.25f, 0.00f, 0.0f, 1.0f, 
    0.25f, 0.25f, 0.0f, 1.0f, 
    0.00f, 0.25f, 0.0f, 1.0f
};

const GLuint squareIndices[] =
{
        0,1,2,
        2,3,0,
};

const GLfloat paralel[] =
{
    0.25f, 0.00f, 0.0f, 1.0f,
    0.50f, 0.00f, 0.0f, 1.0f,
    0.25f, 0.25f, 0.0f, 1.0f, 
    0.00f, 0.25f, 0.0f, 1.0f, 
   -0.25f,0.25f, 0.0f, 1.0f,
    0.00f, 0.00f, 0.0f, 1.0f,
};

const GLuint paralelIndices[] =
{
        0,1,2,

        2,3,0,
        3,4,5,

        5,0,3
};

//Object triangle = Object(0);
//Object square = Object(1);

void createBufferObjects()
{
	glGenVertexArrays(3, VaoId);
	glBindVertexArray(VaoId[0]);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, 0, 0);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);
	}
	glBindVertexArray(VaoId[1]);
	{
		glGenBuffers(2, squareVboId);

		glBindBuffer(GL_ARRAY_BUFFER, squareVboId[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, 0, 0);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareVboId[1]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);
	}
	glBindVertexArray(VaoId[2]);
	{
		glGenBuffers(2, paralelVboId);

		glBindBuffer(GL_ARRAY_BUFFER, paralelVboId[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(paralel), paralel, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, 0, 0);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, paralelVboId[1]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(paralelIndices), paralelIndices, GL_STATIC_DRAW);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //triangle.sendDataToBuffers();
    //square.sendDataToBuffers();

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
Matrix4 _M0 = MatrixFactory::CreateTransformMatrix( -0.15f,-0.2f,0.0f,170.0f,2.0f);
 const float* M0 = _M0.getData();

Matrix4 _M3 = MatrixFactory::CreateTransformMatrix( 0.35f,-0.28f,0.0,125.0f,2.0f);
 const float* M3 = _M3.getData();

// medium triangle
Matrix4 _M4 = MatrixFactory::CreateTransformMatrix( -0.13f,0.40f,0.0,78.0f,1.5f);
 const float* M4 = _M4.getData();

// small triangles
Matrix4 _M5 = MatrixFactory::CreateTransformMatrix( 0.15f,-0.55f,0.0,80.0f,1.2f);
 const float* M5 = _M5.getData();

Matrix4 _M6 = MatrixFactory::CreateTransformMatrix( -0.97f,-0.2f,0.0f,-12.0f,1.2f);
 const float* M6 = _M6.getData();

// square
Matrix4 _M1 = MatrixFactory::CreateTransformMatrix(-0.51f,0.41f,0.0f,33.0f,1.25f);
 const float* M1 = _M1.getData();

 // pararelogram
Matrix4 _M2 = MatrixFactory::CreateTransformMatrix(-0.12f,0.4f,0.0f,-56.0f,1.0f);
 const float* M2 = _M2.getData();

void drawScene()
{
    //test.print();
    program.use();

    // triangles
    glBindVertexArray(VaoId[0]);

    // big triangles
    program.setUniform("Matrix",M0);
    program.setUniform("ex_Color",1,0,0,0);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)0);

    program.setUniform("Matrix",M3);
    program.setUniform("ex_Color",1,1,1,0);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)0);

    // medium triangle
    program.setUniform("Matrix",M4);
    program.setUniform("ex_Color",0.4,0,0.25,0);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)0);

    //small triangles
    
    program.setUniform("Matrix",M5);
    program.setUniform("ex_Color",1,0.5,0,0);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)0);

    program.setUniform("Matrix",M6);
    program.setUniform("ex_Color",0.7,0.7,0.7,0);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)0);

    // square
    glBindVertexArray(VaoId[1]);
    program.setUniform("Matrix",M1);
    program.setUniform("ex_Color",0,0,1,0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);
 
    // pararelogram
    glBindVertexArray(VaoId[2]);
    program.setUniform("Matrix",M2);
    program.setUniform("ex_Color",0,1,0,0);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (GLvoid*)0);

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

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer,0);
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
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}
