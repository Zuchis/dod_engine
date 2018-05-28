#include "../engine.h"
#include "../objects.h" 

#include <algorithm>
#include <time.h>
#include <unistd.h>

#define MIN_SPEED -2
#define MAX_SPEED 2

using namespace engine;

using std::vector;

typedef vector<Vector3> vectors;
typedef vector<Matrix4> matrices;

float xInf = -20.0f;
float xSup =  20.0f;
float yInf = -20.0f;
float ySup =  20.0f;
float zInf = -20.0f;
float zSup =  20.0f;

float lastTime = 0;
float currentTime = 0;
float timeForAdding = 3;

int frameCounter = 0;
int totalFrames = 1000;

const size_t nPrograms = 1;
const size_t nMeshes =  4;
const size_t nObjects = 25;
const size_t totalObjects = (nObjects * 4) + 1;

const size_t squareID = 0;
const size_t ballID = 1;
const size_t parallelID = 2;
const size_t suzanneID = 3;

vector<MeshData> moldMeshes;
vector<ShaderData> moldPrograms;

ObjectLabels gLabels;
NodeTags gTags;

ObjectModels gModels;
ObjectPhysics gPhysics;
NodeData gNodes;

ArcballCamera* graphCamera;

void createShaderProgram()
{
    moldPrograms.reserve(nPrograms);

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

    moldPrograms[0] = ShaderData { .programID = programID, .matrixID = matrixID } ;

    graphCamera = new ArcballCamera(UBO_BP);

    //checkOpenGLError("ERROR: Could not create shaders.");
}

void createMeshes()
{
    moldMeshes.reserve(4);

    std::string tamSquare("objects/tamSquare.obj");
    std::string sphere("objects/sphere.obj");
    std::string parallelogram("objects/tamParalel.obj");
    std::string suzanneStr("objects/suzanne.obj");

    MeshCreator square   = MeshCreator(tamSquare);
    MeshCreator ball     = MeshCreator(sphere);
    MeshCreator parallel = MeshCreator(parallelogram);
    MeshCreator suzanne  = MeshCreator(suzanneStr);

    moldMeshes[squareID] = square.create();
    moldMeshes[ballID] = ball.create();
    moldMeshes[parallelID] = parallel.create();
    moldMeshes[suzanneID] = suzanne.create();

    //checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

float nextRandom(float lo, float hi) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = hi - lo;
    float r = random * diff;
    return lo + r;
}

void reserveModels(size_t size) {
    gModels.translations = (Vector3*)malloc(sizeof(Vector3) * size);
    gModels.rotations = (Quaternion*)malloc(sizeof(Quaternion) * size);
    gModels.scales = (Vector3*)malloc(sizeof(Vector3) * size);
}

void reservePhysics(size_t size) {
    gPhysics.speeds = (Vector3*)malloc(sizeof(Vector3) * size);
    gPhysics.accelerations = (Vector3*)malloc(sizeof(Vector3) * size);
}

void reserveNodes(size_t size) {
    gNodes.parents = (size_t*)malloc(sizeof(size_t) * size);
    gNodes.locals = (Matrix4*)malloc(sizeof(Matrix4) * size);
    gNodes.globals = (Matrix4*)malloc(sizeof(Matrix4) * size);
    gNodes.shaders = (ShaderData*)malloc(sizeof(ShaderData) * size);
    gNodes.meshes = (MeshData*)malloc(sizeof(MeshData) * size);
}

void calculateLocals(Vector3* translations, Quaternion* rotations, Vector3* scales, Matrix4* out_locals) {
    size_t i;
    for (i = 1; i < totalObjects; i++) {
       out_locals[i] =  math::translate(translations[i]) *
                        rotations[i].toMatrix() *
                        math::scale(scales[i]);
    }
}

void calculateGlobals(Matrix4* locals, Matrix4* globals, size_t* parents) {
    size_t i;
    for (i = 1; i < totalObjects; i++) {
       globals[i] = globals[parents[i]] * locals[i];
    }
}

void drawNodes(Matrix4* globals, ShaderData* shaders, MeshData* meshes) {
    size_t i;
    for (i = 1; i < totalObjects; i++) {
       glUseProgram(shaders[i].programID);
       glUniformMatrix4fv(shaders[i].matrixID, 1, GL_FALSE, globals[i].data.data());
       glBindVertexArray(meshes[i].vaoId);
       glDrawArrays(GL_TRIANGLES, 0, meshes[i].nVertices);
    }
}

void updateAccelerations(Vector3* accelerations) {
    size_t i;
    float x, y, z;
    float lo = -0.01f;
    float hi = 0.01f;
    for(i = 1; i < totalObjects; i++) {
        x = nextRandom(lo, hi);
        y = nextRandom(lo, hi);
        z = nextRandom(lo, hi);
        accelerations[i] = Vector3(x, y, z);
    }
}

void updateSpeeds(Vector3* speeds, Vector3* accelerations) {
    size_t i;

    for (i = 1; i < totalObjects; i++) {
        math::clampVector(speeds[i] + accelerations[i], (float)MIN_SPEED, (float)MAX_SPEED);
    }
}

void updateTranslations(Vector3* translations, Vector3* speeds) {
    size_t i;

    for (i = 1; i < totalObjects; i++) {
        translations[i] = translations[i] + speeds[i];
    }
}

void updateRotations(Quaternion* rotations, Vector3* speeds) {
    size_t i;

    for (i = 1; i < totalObjects; i++) {
        Quaternion rotationQtrnY = Quaternion(speeds[i].x * 100, math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
        Quaternion rotationQtrnX = Quaternion(-speeds[i].y * 100, math::Vector4(1.0f, 0.0f, 0.0f, 1.0f));

        rotations[i] = rotationQtrnX * rotationQtrnY * rotations[i];
    }
}

void calculateObjectsCollisionsWithBox(Vector3* translations, Vector3* speeds, Vector3* accelerations) {
    size_t i;
    float bounceAmount = 0.01f;

    for(i = 1; i < totalObjects; i++) {
        Vector3& translation = translations[i];
        Vector3& speed = speeds[i];
        Vector3& acceleration = accelerations[i];

        if (translation.x < xInf) {
            speed = Vector3(-speed.x, speed.y, speed.z);
            acceleration = Vector3(-acceleration.x, acceleration.y , acceleration.z);
            translation.x = translation.x + bounceAmount;
            continue;
        }

        if (translation.x > xSup) {
            speed = Vector3(-speed.x, speed.y, speed.z);
            acceleration = Vector3(-acceleration.x, acceleration.y , acceleration.z);
            translation.x = translation.x - bounceAmount;
            continue;
        }

        if (translation.y < yInf) {
            speed = Vector3(speed.x, -speed.y, speed.z);
            acceleration = Vector3(acceleration.x, -acceleration.y , acceleration.z);
            translation.y = translation.y + bounceAmount;
            continue;
        }

        if (translation.y > ySup) {
            speed = Vector3(speed.x, -speed.y, speed.z);
            acceleration = Vector3(acceleration.x, -acceleration.y , acceleration.z);
            translation.y = translation.y - bounceAmount;
            continue;
        }

        if (translation.z < zInf) {
            speed = Vector3(speed.x, speed.y, -speed.z);
            acceleration = Vector3(acceleration.x, acceleration.y , -acceleration.z);
            translation.z = translation.z + bounceAmount;
            continue;
        }

        if (translation.z > zSup) {
            speed = Vector3(speed.x, speed.y, -speed.z);
            acceleration = Vector3(acceleration.x, acceleration.y , -acceleration.z);
            translation.z = translation.z - bounceAmount;
            continue;
        }
    }
}

void setViewProjectionMatrix() {
    Matrix4 translation = math::translate(Vector3(0.0f,0.0f,(cameraDistance * -1)));
    Matrix4 rotation    = rotationQuaternion.toMatrix();
    graphCamera->setViewMatrix(translation * rotation);
}

void drawScene()
{
    setViewProjectionMatrix();

    calculateLocals(gModels.translations, gModels.rotations, gModels.scales, gNodes.locals);

    calculateGlobals(gNodes.locals, gNodes.globals, gNodes.parents);

    drawNodes(gNodes.globals, gNodes.shaders, gNodes.meshes);

    glUseProgram(0);
    glBindVertexArray(0);
}

void createSceneGraph() {
    size_t i;
    float lo = xInf + 1;
    float hi = xSup - 1;
    float x, y, z;

    std::string iter;

    graphCamera->setProjectionMatrix(
        math::Perspective(30.0f, winWidth / winHeight, 0.1f, 1000.0f));

    reserveNodes(totalObjects);

    reserveModels(totalObjects);

    reservePhysics(totalObjects);

    gModels.translations[0] = Vector3();
    gModels.rotations[0] = Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f));
    gModels.scales[0] = Vector3(1.0f, 1.0f, 1.0f);

    gPhysics.speeds[0] = Vector3();
    gPhysics.accelerations[0] = Vector3();

    gNodes.meshes[0] = moldMeshes[0];
    gNodes.shaders[0] = moldPrograms[0];
    gNodes.parents[0] = (size_t)0;
    gNodes.locals[0] = math::Create4DIdentity();
    gNodes.globals[0] = math::Create4DIdentity();

    size_t objIndex = 1;

    // PLANES

    for (i = 0; i < nObjects; i++) {
        x = nextRandom(lo, hi);
        y = nextRandom(lo, hi);
        z = nextRandom(lo, hi);

        gModels.translations[objIndex] = Vector3(x, y, z);
        gModels.rotations[objIndex] = Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f));
        gModels.scales[objIndex] = Vector3(5.0f, 0.5f, 5.0f);

        gPhysics.speeds[objIndex] = Vector3(0.001f, 0.002f, 0.003f);
        gPhysics.accelerations[objIndex] = Vector3();

        gNodes.meshes[objIndex] = moldMeshes[squareID];
        gNodes.shaders[objIndex] = moldPrograms[0];
        gNodes.parents[objIndex] = (size_t)0;
        gNodes.locals[objIndex] = math::Create4DIdentity();
        gNodes.globals[objIndex] = math::Create4DIdentity();

        objIndex++;
    }

    // ------------------------------------------------//
    
    // SPHERES

    for (i = 0; i < nObjects; i++) {
        x = nextRandom(lo, hi);
        y = nextRandom(lo, hi);
        z = nextRandom(lo, hi);

        gModels.translations[objIndex] = Vector3(x, y, z);
        gModels.rotations[objIndex] = Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f));
        gModels.scales[objIndex] = Vector3(1.0f, 1.0f, 1.0f);

        gPhysics.speeds[objIndex] = Vector3(0.01f, 0.02f, 0.03f);
        gPhysics.accelerations[objIndex] = Vector3();

        gNodes.meshes[objIndex] = moldMeshes[ballID];
        gNodes.shaders[objIndex] = moldPrograms[0];
        gNodes.parents[objIndex] = (size_t)0;
        gNodes.locals[objIndex] = math::Create4DIdentity();
        gNodes.globals[objIndex] = math::Create4DIdentity();

        objIndex++;
    }

    // ------------------------------------------------//
    
    // Parallels

    for (i = 0; i < nObjects; i++) {
        x = nextRandom(lo, hi);
        y = nextRandom(lo, hi);
        z = nextRandom(lo, hi);

        gModels.translations[objIndex] = Vector3(x, y, z);
        gModels.rotations[objIndex] = Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f));
        gModels.scales[objIndex] = Vector3(1.0f,1.0f,0.0f);

        gPhysics.speeds[objIndex] = Vector3(0.005f, 0.01f, 0.015f);
        gPhysics.accelerations[objIndex] = Vector3();

        gNodes.meshes[objIndex] = moldMeshes[parallelID];
        gNodes.shaders[objIndex] = moldPrograms[0];
        gNodes.parents[objIndex] = (size_t)0;
        gNodes.locals[objIndex] = math::Create4DIdentity();
        gNodes.globals[objIndex] = math::Create4DIdentity();

        objIndex++;
    }

    // ------------------------------------------------//
    
    // Suzannes

    for (i = 0; i < nObjects; i++) {
        x = nextRandom(lo, hi);
        y = nextRandom(lo, hi);
        z = nextRandom(lo, hi);

        gModels.translations[objIndex] = Vector3(x, y, z);
        gModels.rotations[objIndex] = Quaternion(0.0f,Vector3(-1.0f,0.0f,0.0f));
        gModels.scales[objIndex] = Vector3(1.0f, 1.0f, 1.0f);

        gPhysics.speeds[objIndex] = Vector3(0.015f, 0.025f, 0.0315f);
        gPhysics.accelerations[objIndex] = Vector3();

        gNodes.meshes[objIndex] = moldMeshes[suzanneID];
        gNodes.shaders[objIndex] = moldPrograms[0];
        gNodes.parents[objIndex] = (size_t)0;
        gNodes.locals[objIndex] = math::Create4DIdentity();
        gNodes.globals[objIndex] = math::Create4DIdentity();

        objIndex++;
    }
}

void computePhysics()
{
    updateAccelerations(gPhysics.accelerations);

    updateSpeeds(gPhysics.speeds, gPhysics.accelerations);

    updateTranslations(gModels.translations, gPhysics.speeds);

    updateRotations(gModels.rotations, gPhysics.speeds);

    calculateObjectsCollisionsWithBox(gModels.translations, gPhysics.speeds, gPhysics.accelerations);
}

void display()
{
    if (frameCounter <= totalFrames + 1) {
        ++FrameCount;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        computeTime();
        computePhysics();
        drawScene();
        glutSwapBuffers();
        frameCounter++;
    } else {
        std::cout << "Parando após " << totalFrames << " execuções" << std::endl;
        glutDestroyWindow(currentWindow);
        exit(0);
    }
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
    init(argc, argv);
    glutMainLoop();
    exit(EXIT_SUCCESS);
}
