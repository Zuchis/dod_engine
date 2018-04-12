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

//int lol;

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

void createMeshes()
{
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

    MeshManager::instance()->add("square",square);
    MeshManager::instance()->add("sphere",ball);
    MeshManager::instance()->add("parallel",parallel);
    MeshManager::instance()->add("suzanne",suzanne);

    checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void createObjects()
{
    Mesh* squareMesh = MeshManager::instance()->get("square");
    Mesh* ballMesh = MeshManager::instance()->get("sphere");
    Mesh* parallelMesh = MeshManager::instance()->get("parallel");
    Mesh* suzanneMesh = MeshManager::instance()->get("suzanne");

    Plane* plane = new Plane("plane");
    plane->setMesh(squareMesh);
    plane->speed = Vector3(0.001f, 0.002f, 0.003f);

    Sphere* ball = new Sphere("ball");
    ball->setMesh(ballMesh);
    ball->speed = Vector3(0.01f, 0.02f, 0.03f);

    Parallelogram* parallel = new Parallelogram("parallel");
    parallel->setMesh(parallelMesh);
    parallel->speed = Vector3(0.005f, 0.01f, 0.015f);

    Suzanne* suzanne = new Suzanne("suzanne");
    suzanne->setMesh(suzanneMesh);
    suzanne->speed = Vector3(0.015f, 0.025f, 0.0315f);

    ObjectManager::instance()->add("plane", plane);
    ObjectManager::instance()->add("ball", ball);
    ObjectManager::instance()->add("parallel", parallel);
    ObjectManager::instance()->add("suzanne", suzanne);
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
    int i;
    int nObjects = 3;
    float lo = xInf + 1;
    float hi = xSup - 1;
    float x, y, z;

    std::string iter;

    Mesh* squareMesh = MeshManager::instance()->get("square");
    Mesh* ballMesh = MeshManager::instance()->get("sphere");
    Mesh* parallelMesh = MeshManager::instance()->get("parallel");
    Mesh* suzanneMesh = MeshManager::instance()->get("suzanne");

    SceneGraph* scenegraph = new SceneGraph();
    //camera = new ArcballCamera(UBO_BP);
    scenegraph->setCamera(new ArcballCamera(UBO_BP));

    scenegraph->getCamera()->setProjectionMatrix(
        math::Perspective(30.0f, winWidth / winHeight, 0.1f, 1000.0f));

    root = scenegraph->getRoot();
    root->setShaderProgram(ShaderProgramManager::instance()->get("default"));

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

        ObjectManager::instance()->add("plane" + iter, plane);

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

        ObjectManager::instance()->add("sphere" + iter, sphere);

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

        ObjectManager::instance()->add("parallel" + iter, parallel);

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

        ObjectManager::instance()->add("suzanne" + iter, suzanne);
    }

    SceneGraphManager::instance()->add("default",scenegraph);
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
    SceneGraphManager::instance()->get("default")->getCamera()->setViewMatrix(translation * rotation);
}

void updateAccelerations() {
    auto objects = ObjectManager::instance()->getObjects();
    auto it = objects.begin();
    float x, y, z;
    float lo = -0.01f;
    float hi = 0.01f;
    for(it = objects.begin(); it != objects.end(); it++) {
        x = nextRandom(lo, hi);
        y = nextRandom(lo, hi);
        z = nextRandom(lo, hi);
        it->second->acceleration = Vector3(x, y, z);
    }
}

void addRandomObject() {
    static int nRandom = 0;
    vector<char> options = {'0', '1', '2', '3'};
    std::random_shuffle(options.begin(), options.end());
    char chosen = options[0];
    std::string iter = std::to_string(nRandom);
    auto scenegraph = SceneGraphManager::instance()->get("default");
    float x, y, z;
    float lo = xInf + 1;
    float hi = xSup - 1;

    x = nextRandom(lo, hi);
    y = nextRandom(lo, hi);
    z = nextRandom(lo, hi);

    switch (chosen) {
        case '0':
          {
            Plane* plane = new Plane("plane");
            plane->setMesh(MeshManager::instance()->get("square"));
            plane->speed = Vector3(0.001f, 0.002f, 0.003f);
            plane->setTranslation(Vector3(x, y, z));
            plane->setRotation(Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f)));
            plane->setScale(Vector3(5.0f, 0.5f, 5.0f));
            ObjectManager::instance()->add("random" + iter, plane);
            SceneNode* nodePlane = scenegraph->createNode("plane");
            nodePlane->setObject(plane);
            planeNodes.push_back(nodePlane);
            break;
          }

        case '1':
          {
            Sphere* sphere = new Sphere("ball");
            sphere->setMesh(MeshManager::instance()->get("sphere"));
            sphere->speed = Vector3(0.01f, 0.02f, 0.03f);
            sphere->setTranslation(Vector3(x,y,z));
            sphere->setRotation(Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f)));
            sphere->setScale(Vector3(1.0f, 1.0f, 1.0f));
            ObjectManager::instance()->add("random" + iter, sphere);
            SceneNode* nodeSphere = scenegraph->createNode("sphere");
            nodeSphere->setObject(sphere);
            sphereNodes.push_back(nodeSphere);
            break;
          }

        case '2':
          {
            Parallelogram* parallel = new Parallelogram("parallel");
            parallel->setMesh(MeshManager::instance()->get("parallel"));
            parallel->speed = Vector3(0.01f, 0.02f, 0.03f);
            parallel->setTranslation(Vector3(x,y,z));
            parallel->setRotation(Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f)));
            parallel->setScale(Vector3(1.0f,1.0f,0.0f));
            ObjectManager::instance()->add("random" + iter, parallel);
            SceneNode* nodeParallel = scenegraph->createNode("parallel");
            nodeParallel->setObject(parallel);
            parallelNodes.push_back(nodeParallel);
            break;
          }

        case '3':
          {
            Suzanne* suzanne = new Suzanne("suzanne");
            suzanne->setMesh(MeshManager::instance()->get("suzanne"));
            suzanne->speed = Vector3(0.01f, 0.02f, 0.03f);
            suzanne->setTranslation(Vector3(x,y,z));
            suzanne->setRotation(Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f)));
            suzanne->setScale(Vector3(1.0f,1.0f,0.0f));
            ObjectManager::instance()->add("random" + iter, suzanne);
            SceneNode* nodeSuzanne = scenegraph->createNode("suzanne");
            nodeSuzanne->setObject(suzanne);
            suzanneNodes.push_back(nodeSuzanne);
            break;
          }

        default:
            std::cout << "WHAT" << std::endl;
    }
    
    nRandom++;
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
        addRandomObject();
        updateAccelerations();
    }

    setViewProjectionMatrix();
    SceneGraphManager::instance()->get("default")->draw();

    glUseProgram(0);
    glBindVertexArray(0);
}

void computeCameraMovement() 
{
    if(KeyBuffer::instance()->isKeyDown('w'))
        camera->computeKeyboardInputs(FORWARD,deltaTime);

    if(KeyBuffer::instance()->isKeyDown('s'))
        camera->computeKeyboardInputs(BACKWARD,deltaTime);

    if(KeyBuffer::instance()->isKeyDown('a'))
        camera->computeKeyboardInputs(LEFT,deltaTime);

    if(KeyBuffer::instance()->isKeyDown('d'))
        camera->computeKeyboardInputs(RIGHT,deltaTime);

    //if(zoomFactor != 0.0f)
        //camera->computeMouseScroll(zoomFactor);

    if (deltaX != 0 || deltaY != 0)
        camera->computeMouseMovement(deltaX, -deltaY);
    
}

void computeInputs() 
{
    computeCameraMovement();
}

void computePhysics()
{
    ObjectManager::instance()->updateObjects();

    ObjectManager::instance()->calculateObjectsCollisionsWithBox(xInf, xSup, yInf, ySup, zInf, zSup);
}

void display()
{
    ++FrameCount;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    computeTime();
    //computeInputs();
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

    createObjects();

    createSceneGraph();
}

int main(int argc, char* argv[])
{
    std::cout << "oi" << std::endl;
    init(argc, argv);
    glutMainLoop();
    exit(EXIT_SUCCESS);
}
