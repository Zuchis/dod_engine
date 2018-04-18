#include "../engine.h"
#include "../objects.h" 

#include <algorithm>

using namespace engine;

using std::vector;

FPSCamera* camera;

float xInf = -20.0f;
float xSup =  20.0f;
float yInf = -20.0f;
float ySup =  20.0f;
float zInf = -20.0f;
float zSup =  20.0f;

const size_t nPrograms = 1;
const size_t nMeshes =  4;
const size_t nObjects = 3;
const size_t totalObjects = nObjects * 4;

const size_t squareID = 0;
const size_t ballID = 1;
const size_t parallelID = 2;
const size_t suzanneID = 3;

vector<Mesh*> meshes;
vector<ShaderData> programs;
vector<Object*> objects;

SceneGraph* graph;

void createShaderProgram()
{
    programs.reserve(nPrograms);

    GLuint programID = glCreateProgram();
    GLint matrixID;

    compileShaderFromFile(programID, "shaders/vertexShaderSceneManagement.vert",ShaderType::VERTEX);

    compileShaderFromFile(programID, "shaders/fragmentShaderSceneManagement.frag",ShaderType::FRAGMENT);

    bindAttribLocation(programID, VERTICES,"in_Position");
    bindAttribLocation(programID, TEXCOORDS,"in_TexCoords");
    bindAttribLocation(programID, NORMALS,"in_Normal");
    addUniformBlock(programID, "Camera", UBO_BP);

    link(programID);

    matrixID = getMatrixID(programID);

    programs[0] = ShaderData { .programID = programID, .matrixID = matrixID } ;

    //checkOpenGLError("ERROR: Could not create shaders.");
}

void createMeshes()
{
    meshes.reserve(4);

    std::string tamSquare("objects/tamSquare.obj");
    std::string sphere("objects/sphere.obj");
    std::string parallelogram("objects/tamParalel.obj");
    std::string suzanneStr("objects/suzanne.obj");

    Mesh* square = new Mesh(tamSquare);
    Mesh* ball = new Mesh(sphere);
    Mesh* parallel = new Mesh(parallelogram);
    Mesh* suzanne = new Mesh(suzanneStr);

    square->create();
    ball->create();
    parallel->create();
    suzanne->create();

    meshes[0] = square;
    meshes[1] = ball;
    meshes[2] = parallel;
    meshes[3] = suzanne;

    meshes[squareID] = square;
    meshes[ballID] = ball;
    meshes[parallelID] = parallel;
    meshes[suzanneID] = suzanne;

    checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

float nextRandom(float lo, float hi) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = hi - lo;
    float r = random * diff;
    return lo + r;
}

SceneNode* root;

vector<SceneNode*> planeNodes;
vector<SceneNode*> sphereNodes;
vector<SceneNode*> parallelNodes;
vector<SceneNode*> suzanneNodes;

void createSceneGraph() {
    size_t i;
    float lo = xInf + 1;
    float hi = xSup - 1;
    float x, y, z;

    std::string iter;

    Mesh* squareMesh = meshes[squareID];
    Mesh* ballMesh = meshes[ballID];
    Mesh* parallelMesh = meshes[parallelID];
    Mesh* suzanneMesh = meshes[suzanneID];

    SceneGraph* scenegraph = new SceneGraph();
    scenegraph->setCamera(new ArcballCamera(UBO_BP));

    scenegraph->getCamera()->setProjectionMatrix(
        math::Perspective(30.0f, winWidth / winHeight, 0.1f, 1000.0f));

    root = scenegraph->getRoot();
    root->shaderData = programs[0];

    objects.reserve(totalObjects);

    size_t objIndex = 0;

    for (i = 0; i < nObjects; i++) {
        iter = std::to_string(i);

        x = nextRandom(lo, hi);
        y = nextRandom(lo, hi);
        z = nextRandom(lo, hi);

        Plane* plane = new Plane("plane" + iter);
        plane->setMesh(squareMesh);
        plane->speed = Vector3(0.001f, 0.002f, 0.003f);
        plane->setTranslation(Vector3(x, y, z));
        plane->setRotation(Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f)));
        plane->setScale(Vector3(5.0f, 0.5f, 5.0f));

        planeNodes.push_back(scenegraph->createNode("plane"));
        planeNodes[i]->setObject(plane);
        planeNodes[i]->shaderData = programs[0];

        objects[objIndex] = plane;
        objIndex++;

        x = nextRandom(lo, hi);
        y = nextRandom(lo, hi);
        z = nextRandom(lo, hi);

        Sphere* sphere = new Sphere("ball" + iter);
        sphere->setMesh(ballMesh);
        sphere->speed = Vector3(0.01f, 0.02f, 0.03f);
        sphere->setTranslation(Vector3(x,y,z));
        sphere->setRotation(Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f)));
        sphere->setScale(Vector3(1.0f, 1.0f, 1.0f));

        sphereNodes.push_back(scenegraph->createNode("ball"));
        sphereNodes[i]->setObject(sphere);
        sphereNodes[i]->shaderData = programs[0];

        objects[objIndex] = sphere;
        objIndex++;

        x = nextRandom(lo, hi);
        y = nextRandom(lo, hi);
        z = nextRandom(lo, hi);

        Parallelogram* parallel = new Parallelogram("parallel" + iter);
        parallel->setMesh(parallelMesh);
        parallel->speed = Vector3(0.005f, 0.01f, 0.015f);
        parallel->setTranslation(Vector3(x,y,z));
        parallel->setRotation(Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f)));
        parallel->setScale(Vector3(1.0f,1.0f,0.0f));

        parallelNodes.push_back(scenegraph->createNode("parallel"));
        parallelNodes[i]->setObject(parallel);
        parallelNodes[i]->shaderData = programs[0];

        objects[objIndex] = parallel;
        objIndex++;

        x = nextRandom(lo, hi);
        y = nextRandom(lo, hi);
        z = nextRandom(lo, hi);

        Suzanne* suzanne = new Suzanne("suzanne" + iter);
        suzanne->setMesh(suzanneMesh);
        suzanne->speed = Vector3(0.015f, 0.025f, 0.0315f);
        suzanne->setTranslation(Vector3(x,y,z));
        suzanne->setRotation(Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f)));
        suzanne->setScale(Vector3(1.0f,1.0f,0.0f));

        suzanneNodes.push_back(scenegraph->createNode("parallel"));
        suzanneNodes[i]->setObject(suzanne);
        suzanneNodes[i]->shaderData = programs[0];

        objects[objIndex] = suzanne;
        objIndex++;
    }

    graph = scenegraph;
}

//void setViewProjectionMatrix() 
//{
    //Matrix4 viewMatrix = camera->getViewMatrix();

    //camera->setProjectionMatrix(
        //math::Perspective(math::toRadians(camera->zoom), winWidth / winHeight, 0.1f, 1000.0f));

    //camera->setViewMatrix(viewMatrix);
//}

void setViewProjectionMatrix() {
    Matrix4 translation = math::translate(Vector3(0.0f,0.0f,(cameraDistance * -1)));
    Matrix4 rotation    = rotationQuaternion.toMatrix();
    graph->getCamera()->setViewMatrix(translation * rotation);
}

void updateAccelerations() {
    size_t i;
    float x, y, z;
    float lo = -0.01f;
    float hi = 0.01f;
    for(i = 0; i < totalObjects; i++) {
        x = nextRandom(lo, hi);
        y = nextRandom(lo, hi);
        z = nextRandom(lo, hi);
        objects[i]->acceleration = Vector3(x, y, z);
    }
}

float lastTime = 0;
float currentTime = 0;

float timeForAdding = 3;

void drawScene()
{
    currentTime = timeSinceStart();
    if (currentTime - lastTime > timeForAdding) {
        lastTime = currentTime;

        std::cout << "Ta pegando fogo bixo" << std::endl;
        updateAccelerations();
    }

    setViewProjectionMatrix();
    graph->draw();

    glUseProgram(0);
    glBindVertexArray(0);
}

void updateObjects() {
    size_t i;

    for(i = 0; i < totalObjects; i++) {
        objects[i]->update();
    }
}

void calculateObjectsCollisionsWithBox(float xInf, float xSup, float yInf, float ySup, float zInf, float zSup) {
    size_t i;

    for(i = 0; i < totalObjects; i++) {
        objects[i]->calculateCollisionsWithBox(xInf, xSup, yInf, ySup, zInf, zSup);
    }
}

void computePhysics()
{
    updateObjects();

    calculateObjectsCollisionsWithBox(xInf, xSup, yInf, ySup, zInf, zSup);
}

void display()
{
    ++FrameCount;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    computeTime();
    computePhysics();
    drawScene();
    glutSwapBuffers();
}

void init(int argc, char* argv[])
{
    engine_init(argc,argv);
    glutDisplayFunc(display);

    createMeshes();

    createShaderProgram();

    createSceneGraph();
}

int main(int argc, char* argv[])
{
    std::cout << "oi" << std::endl;
    init(argc, argv);
    glutMainLoop();
    exit(EXIT_SUCCESS);
}
