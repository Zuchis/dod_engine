#include "../engine.h"

using namespace engine;

using math::Quaternion;

#define CAPTION "Badass Moving Squirrel"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

GLfloat lastX = (GLfloat)(WinX / 2);
GLfloat lastY = (GLfloat)(WinY / 2);
GLfloat currentX, currentY;

GLuint UBO_BP = 0;

bool leftMouseButtonPressed = false;

float groundY, groundZ, groundX = 0.0f;
float groundRot = 0.0f;
float distance = 15;
float tamHeight = 7.0f;

float animationDuration = 2.0f;
float interpolationFactor = 1.0f / animationDuration;
float currentInterpolation = 0.0f;
bool  interpolationSwitch = false;

float maxSpeed = 1.0f;
float maxRotationSpeed = 3.0f;
float distanceStep = 1.5f;

GLfloat deltaTime = 0.0f;
GLfloat oldTime = 0.0f;

Quaternion rotationQuaternion(1.0f,0.0f,0.0f,0.0f);

std::unordered_map<std::string,Vector3> squirrelPositions, boxPositions;
std::unordered_map<std::string,Quaternion> squirrelRotations, boxRotations;
std::unordered_map<std::string,Vector3> pieceScales;
std::unordered_map<std::string,Vector3> boxScales;
std::string activeSceneGraph = "squirrel";

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
    ShaderProgram *program = new ShaderProgram();

    program->compileShaderFromFile("shaders/vertexShaderSceneManagement.vert",ShaderType::VERTEX);

    program->compileShaderFromFile("shaders/fragmentShaderSceneManagement.frag",ShaderType::FRAGMENT);

    program->bindAttribLocation(VERTICES,"in_Position");
    program->bindAttribLocation(TEXCOORDS,"in_TexCoords");
    program->bindAttribLocation(NORMALS,"in_Normal");
    program->addUniformBlock("Camera", UBO_BP);

    program->link();

    ShaderProgramManager::instance()->add("default",program);

    //checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	glUseProgram(0);

	//checkOpenGLError("ERROR: Could not destroy shaders.");
}

void createMeshes()
{

    std::string object("objects/suzanne.obj");
    std::string tamTriangle("objects/tamTriangle.obj");
    std::string tamSquare("objects/tamSquare.obj");
    std::string tamParalel("objects/tamParalel.obj");

    Mesh *suzanne = new Mesh(object);
    Mesh *triangle = new Mesh(tamTriangle);
    Mesh *square = new Mesh(tamSquare);
    Mesh *paralel = new Mesh(tamParalel);

     suzanne->create();
    triangle->create();
      square->create();
     paralel->create();

     MeshManager::instance()->add("suzanne",suzanne);
     MeshManager::instance()->add("triangle",triangle);
     MeshManager::instance()->add("square",square);
     MeshManager::instance()->add("paralel",paralel);
     
    
    checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}


SceneNode *ground, *bigTriangle1, *bigTriangle2, *mediumTriangle, *smallTriangle1, *smallTriangle2, *square1, *paralelogram;
SceneNode* suzanneNode;
SceneNode* root;


void destroyBufferObjects(){
}

void initializeDictionaries(){
    Vector3 v(0.0f,0.0f,0.0f);
    Quaternion q(1.0f,0.0f,0.0f,0.0f);
    Vector3 z_axis(0.0f,0.0f,1.0f);
    float offset = -0.4f;

    // Squirrel
    v = Vector3(-0.31f,-0.3f,1.0f);
    q = Quaternion(165.0f,z_axis);
    squirrelPositions["bigTriangle1"] = v;
    squirrelRotations["bigTriangle1"] = q;

    v = Vector3(0.07f,-0.19f,1.0f);
    q = Quaternion(125.0f,z_axis);
    squirrelPositions["bigTriangle2"] = v;
    squirrelRotations["bigTriangle2"] = q;

    v = Vector3(-0.620f,0.14f,1.0f);
    q = Quaternion(-20.0f,z_axis);
    squirrelPositions["mediumTriangle"] = v;
    squirrelRotations["mediumTriangle"] = q;

    v = Vector3(-0.581f,0.278f,1.0f);
    q = Quaternion(-67.0f,z_axis);
    squirrelPositions["smallTriangle1"] = v;
    squirrelRotations["smallTriangle1"] = q;

    v = Vector3(0.32f,-0.43f,1.0f);
    q = Quaternion(-35.0f,z_axis);
    squirrelPositions["smallTriangle2"] = v;
    squirrelRotations["smallTriangle2"] = q * Quaternion(180.0f,Vector3(-1.0f,0.0f,0.0f));

    v = Vector3(0.33f,0.3f,1.0f);
    q = Quaternion(127.0f,z_axis);
    squirrelPositions["paralelogram"] = v;
    squirrelRotations["paralelogram"] = q * Quaternion(180.0f,Vector3(0.0f,1.0f,0.0f));

    v = Vector3(0.111f,0.64f,1.0f);
    q = Quaternion(165.3f,z_axis);
    squirrelPositions["square"] = v;
    squirrelRotations["square"] = q * Quaternion(180.0f,Vector3(-1.0f,0.0f,0.0f));

    // Box
    
    v = Vector3(0.8f+offset,0.1f,1.0f);
    q = Quaternion(153.0f+60.0f,z_axis);
    boxPositions["bigTriangle1"] = v;
    boxRotations["bigTriangle1"] = q;

    v = Vector3(0.37f+offset,0.522f,1.0f);
    q = Quaternion(-60.0f,z_axis);
    boxPositions["bigTriangle2"] = v;
    boxRotations["bigTriangle2"] = q;

    v = Vector3(0.13f+offset,-0.17f,1.0f);
    q = Quaternion(-110.0f,z_axis);
    boxPositions["mediumTriangle"] = v;
    boxRotations["mediumTriangle"] = q;

    v = Vector3(0.38f+offset,-0.145f,1.0f);
    q = Quaternion(145.3f,z_axis);
    boxPositions["smallTriangle1"] = v;
    boxRotations["smallTriangle1"] = q;

    v = Vector3(-0.1f+offset,0.3f,1.0f);
    q = Quaternion(-60.0f,z_axis);
    boxPositions["smallTriangle2"] = v;
    boxRotations["smallTriangle2"] = q * Quaternion(180.0f,Vector3(-1.0f,0.0f,0.0f));

    v = Vector3(0.46f+offset,-0.237f,1.0f);
    q = Quaternion(-179.0f,z_axis);
    boxPositions["paralelogram"] = v;
    boxRotations["paralelogram"] = q* Quaternion(180.0f,Vector3(0.0f,1.0f,0.0f));

    v = Vector3(0.15f+offset,0.1f,1.0f);
    q = Quaternion(212.3f,z_axis);
    boxPositions["square"] = v;
    boxRotations["square"] = q;

    // scales
    
    pieceScales["bigTriangle"] = Vector3(0.3f,0.3f,0.3f);
    pieceScales["mediumTriangle"] = Vector3(0.20f,0.20f,0.20f); // 16
    pieceScales["smallTriangle"] = Vector3(0.1f,0.1f,0.1f);
    pieceScales["paralelogram"] = Vector3(0.15f,0.12f,0.12f); //0.1
    pieceScales["square"] = Vector3(0.13f,0.17f,0.13f);

    boxScales["bigTriangle"] = Vector3(0.3f,0.3f,0.3f);
    boxScales["mediumTriangle"] = Vector3(0.23f,0.23f,0.23f); // 16
    boxScales["smallTriangle"] = Vector3(0.15f,0.17f,0.15f);
    boxScales["paralelogram"] = Vector3(0.12f,0.15f,0.12f); //0.1
    boxScales["square"] = Vector3(0.19f,0.17f,0.15f);



}

void createSquirrel() {
    SceneGraph* scenegraph = new SceneGraph();
    scenegraph->setCamera(new ArcballCamera(UBO_BP));

    scenegraph->getCamera()->setProjectionMatrix(
            math::Perspective(30.0f, WinX / WinY, 0.1f, 100.0f));
    
    Mesh* squareMesh = MeshManager::instance()->get("square");
    Mesh* triangleMesh = MeshManager::instance()->get("triangle");
    Mesh* paralelMesh = MeshManager::instance()->get("paralel");

    root = scenegraph->getRoot();
    root->setShaderProgram(ShaderProgramManager::instance()->get("default"));

    ground = scenegraph->createNode("ground");
    ground->setMesh(squareMesh);
    ground->setModelMatrix(math::translate(Vector3(0.0f,0.0f,0.0f)) *
                            Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f)).toMatrix() *
                            math::scale(Vector3(4.0f,3.0f,0.4f)));

    suzanneNode = ground->createNode("suzanne");
    suzanneNode->setMesh(MeshManager::instance()->get("suzanne"));

    bigTriangle1 = ground->createNode("triangle");
    bigTriangle1->setMesh(triangleMesh);
    bigTriangle1->setModelMatrix(math::translate(squirrelPositions["bigTriangle1"]) *
                                 squirrelRotations["bigTriangle1"].toMatrix() *
                                 math::scale(Vector3(0.3f,0.3f,0.3f)));

    bigTriangle2 = ground->createNode("triangle");
    bigTriangle2->setMesh(triangleMesh);
    bigTriangle2->setModelMatrix(math::translate(squirrelPositions["bigTriangle2"]) *
                                 squirrelRotations["bigTriangle2"].toMatrix() *
                                 math::scale(Vector3(0.3f,0.3f,0.3f)));

    mediumTriangle = ground->createNode("triangle");
    mediumTriangle->setMesh(triangleMesh);
    mediumTriangle->setModelMatrix(math::translate(squirrelPositions["mediumTriangle"]) *
                                 squirrelRotations["mediumTriangle"].toMatrix() *
                                 math::scale(Vector3(0.16f,0.16f,0.16f)));

    smallTriangle1 = ground->createNode("triangle");
    smallTriangle1->setMesh(triangleMesh);
    smallTriangle1->setModelMatrix(math::translate(squirrelPositions["smallTriangle1"]) *
                                 squirrelRotations["smallTriangle1"].toMatrix() *
                                 math::scale(Vector3(0.1f,0.1f,0.1f)));

    smallTriangle2 = ground->createNode("triangle");
    smallTriangle2->setMesh(triangleMesh);
    smallTriangle2->setModelMatrix(math::translate(squirrelPositions["smallTriangle2"]) *
                                    squirrelRotations["smallTriangle2"].toMatrix() *
                                    math::scale(Vector3(0.1f,0.1f,0.1f)));

    paralelogram = ground->createNode("paralelogram");
    paralelogram->setMesh(paralelMesh);
    paralelogram->setModelMatrix(math::translate(squirrelPositions["paralelogram"]) *
                                    squirrelRotations["paralelogram"].toMatrix() *
                                    math::scale(Vector3(0.1f,0.1f,0.1f)));

    square1 = ground->createNode("square");
    square1->setMesh(squareMesh);
    square1->setModelMatrix(math::translate(squirrelPositions["square"]) *
                            squirrelRotations["square"].toMatrix() *
                            math::scale(Vector3(0.1f,0.1f,0.1f)));


    SceneNode* eye = mediumTriangle->createNode();
    eye->setMesh(squareMesh);
    eye->applyMatrix(math::translate(Vector3(0.5f,0.0f,1.0f)) *
            math::scale(Vector3(0.1f,0.1f,0.1f)));
    
    ground->applyMatrixToChildren(math::translate(Vector3(0.0f,-0.15f,0.0f)) *
                                    math::scale(Vector3(1.0f,1.5f,1.0f)));
    

    SceneGraphManager::instance()->add("squirrel",scenegraph);
}

void applyGroundMovement() {
    float angleStep = 0.05f * deltaTime;
    float moveStep = 0.0025f * deltaTime;

    if(KeyBuffer::instance()->isSpecialKeyDown(GLUT_KEY_UP) && groundY < maxSpeed)
        groundY += moveStep;
    else if (groundY > 0.0f) 
        groundY -= moveStep;

    if(KeyBuffer::instance()->isSpecialKeyDown(GLUT_KEY_DOWN) && groundY > -maxSpeed)
        groundY -= moveStep;
    else if (groundY < 0.0f)
        groundY += moveStep;

    if(KeyBuffer::instance()->isSpecialKeyDown(GLUT_KEY_LEFT) && groundX > -maxSpeed) 
        groundX -= moveStep;
    else if (groundX < 0.0f)
        groundX += moveStep;

    if(KeyBuffer::instance()->isSpecialKeyDown(GLUT_KEY_RIGHT) && groundX < maxSpeed)
        groundX += moveStep;
    else if (groundX > 0.0f)
        groundX -= moveStep;

    if (KeyBuffer::instance()->isKeyDown('q') && groundRot > -maxSpeed)
        groundRot -= angleStep;
    else if (groundRot < 0.0f)
        groundRot += angleStep;

    if (KeyBuffer::instance()->isKeyDown('e') && groundRot < maxSpeed)
        groundRot += angleStep;
    else if (groundRot > 0.0f)
        groundRot -= angleStep;

    if(KeyBuffer::instance()->isKeyDown('w'))
        tamHeight += 0.005f;

    if(KeyBuffer::instance()->isKeyDown('s'))
        tamHeight -= 0.005f;

}

void computeInterpolation() {
    float interpolationStep = interpolationFactor * deltaTime;

    if(interpolationSwitch)
        currentInterpolation += interpolationStep;
    else
        currentInterpolation -= interpolationStep;

    if(currentInterpolation < 0.0f)
        currentInterpolation = 0.0f;

    if(currentInterpolation > 1.0f)
        currentInterpolation = 1.0f;
}

void applyMotion() {

    applyGroundMovement();

    computeInterpolation();
}

void computeTime() {
    GLfloat currentTime = ((GLfloat)glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
    deltaTime = currentTime - oldTime;
    oldTime = currentTime;
}

void computeAngleAxis(){
    if (currentX != lastX || currentY != lastY) {
        float rotX = currentX - lastX; 
        float rotY = currentY - lastY; 
        Quaternion rotationQtrnY = Quaternion(rotX,math::Vector4(0.0f,1.0f,0.0f,1.0f));
        Quaternion rotationQtrnX = Quaternion(rotY,math::Vector4(1.0f,0.0f,0.0f,1.0f));
        rotationQuaternion = rotationQtrnX * rotationQtrnY * rotationQuaternion;
        lastX = currentX;
        lastY = currentY;
    }
}

void setViewProjectionMatrix() {
    Matrix4 translation = math::translate(Vector3(0.0f,0.0f,-distance));
    Matrix4 rotation    = rotationQuaternion.toMatrix();
    SceneGraphManager::instance()->get(activeSceneGraph)->getCamera()->setViewMatrix(translation * rotation);
}

void applyModelMatrix(SceneNode* node, std::string name, std::string pieceName, float t) {
    Quaternion q(1.0f,0.0f,0.0f,0.0f);
    Vector3 v(0.0f,0.0f,0.0f);
    v = math::lerp(squirrelPositions[name],boxPositions[name],t);
    q = math::lerp(squirrelRotations[name],boxRotations[name],t);
    Vector3 s = math::lerp(pieceScales[pieceName],boxScales[pieceName],t);
    node->setModelMatrix(math::translate(v) *
                      q.toMatrix() * 
                      math::scale(s));

}

void animate(float t) {

    applyModelMatrix(bigTriangle1,"bigTriangle1","bigTriangle",t);

    applyModelMatrix(bigTriangle2,"bigTriangle2","bigTriangle",t);

    applyModelMatrix(mediumTriangle,"mediumTriangle","mediumTriangle",t);

    applyModelMatrix(smallTriangle1,"smallTriangle1","smallTriangle",t);

    applyModelMatrix(smallTriangle2,"smallTriangle2","smallTriangle",t);

    applyModelMatrix(paralelogram,"paralelogram","paralelogram",t);

    applyModelMatrix(square1,"square","square",t);

}

void drawSceneGraph() {
    ground->applyMatrix(math::translate(Vector3(groundX,groundY,groundZ)) *
                            math::rotate(groundRot,Vector3(-1.0f,0.0f,0.0f)));

    animate(currentInterpolation);

    ground->applyMatrixToChildren(math::translate(Vector3(0.0f,1.0f,tamHeight)) *
                                  math::rotate(90.0f,Vector3(1.0f,0.0f,0.0f)) *
                                  math::scale(Vector3(1.0f,10.0f,1.0f)));
    
    suzanneNode->setModelMatrix(math::translate(Vector3(0.0f,3.0f,tamHeight)) *
                                  math::rotate(90.0f,Vector3(1.0f,0.0f,0.0f)) *
                                  math::scale(Vector3(1.0f,10.0f,1.0f)));

    SceneGraphManager::instance()->get(activeSceneGraph)->draw();

}

void drawScene()
{
    computeTime();
    applyMotion();
    setViewProjectionMatrix();
    drawSceneGraph();

    glUseProgram(0);
    glBindVertexArray(0);

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
    KeyBuffer::instance()->pressKey(key);
    if (key == 'g')
        interpolationSwitch = !interpolationSwitch;
}

void keyboardPressSpecial(int key, int x, int y){
    KeyBuffer::instance()->pressSpecialKey(key);
}

void keyboardUp(unsigned char key, int x, int y) {
    KeyBuffer::instance()->releaseKey(key);
}

void keyboardUpSpecial(int key, int x, int y) {
    KeyBuffer::instance()->releaseSpecialKey(key);
}

void mousePress(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        leftMouseButtonPressed = true;
        lastX = currentX = x;
        lastY = currentY = y;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        leftMouseButtonPressed = false;

    if (button == MOUSE_SCROLL_UP){
        distance -= distanceStep;
    }

    if (button == MOUSE_SCROLL_DOWN)
        distance += distanceStep;

}

void mouseMovement(int x, int y){
    if(leftMouseButtonPressed){
        currentX = x;
        currentY = y;
        computeAngleAxis();
    }
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer,0);
        glutKeyboardFunc(keyboardPress);
        glutSpecialFunc(keyboardPressSpecial);
        glutKeyboardUpFunc(keyboardUp);
        glutSpecialUpFunc(keyboardUpSpecial);
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
	setupCallbacks();

	createMeshes();

	createShaderProgram();

        initializeDictionaries();

        createSquirrel();

}

int main(int argc, char* argv[])
{
        init(argc, argv);
        glutMainLoop();
	exit(EXIT_SUCCESS);
}
