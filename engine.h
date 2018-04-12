#ifndef ENGINE_H
#define ENGINE_H

#include "camera.h"
#include "matrixfactory.h"
#include "quaternion.h"
#include "mesh.h"
#include "shaderprogram.h"
#include "managers.h"
#include "object.h"

using math::Quaternion;

namespace engine {

    int currentWindow = 0;
    int winWidth = 1024, winHeight = 768;

    char winCaption[] = "Goodbye UDESC";

    float cameraDistance = 150.0f;
    float cameraDistanceStep = 1.5f;

    unsigned int FrameCount = 0;

    GLfloat deltaTime = 0.0f;
    GLfloat oldTime = 0.0f;

    GLfloat zoomFactor = 1.5f;

    GLfloat lastX = (GLfloat)(winWidth / 2);
    GLfloat lastY = (GLfloat)(winHeight / 2);

    GLfloat currentX = lastX;
    GLfloat currentY = lastY;

    GLfloat deltaX = 0.0f;
    GLfloat deltaY = 0.0f;

    GLuint UBO_BP = 0;

    Quaternion rotationQuaternion(1.0f,0.0f,0.0f,0.0f);

    void setupGLUT(int argc, char* argv[])
    {
        glutInit(&argc, argv);

        glutInitContextVersion(3, 3);
        glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
        glutInitContextProfile(GLUT_CORE_PROFILE);

        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);

        glutInitWindowSize(winWidth, winHeight);
        glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
        currentWindow = glutCreateWindow(winCaption);
        if(currentWindow < 1) {
            std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
            exit(EXIT_FAILURE);
        }
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

    void idle()
    {
        glutPostRedisplay();
    }

    void reshape(int w, int h)
    {
        winWidth = w;
        winHeight = h;
        glViewport(0, 0, winWidth, winHeight);
    }

    void timer(int value)
    {
        std::ostringstream oss;
        oss << winCaption << ": " << FrameCount << " FPS @ (" << winWidth << "x" << winHeight << ")";
        std::string s = oss.str();
        glutSetWindow(currentWindow);
        glutSetWindowTitle(s.c_str());
        FrameCount = 0;
        glutTimerFunc(1000, timer, 0);
    }

    void keyboardPress(unsigned char key, int x, int y) {
        KeyBuffer::instance()->pressKey(key);
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
        zoomFactor = 0.0f;

        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            KeyBuffer::leftMouseButtonPressed = true;
            lastX = currentX = x;
            lastY = currentY = y;
        }

        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
            KeyBuffer::leftMouseButtonPressed = false;

        if (button == MOUSE_SCROLL_UP) {
            cameraDistance -= cameraDistanceStep;
            zoomFactor = 1.5f;
        }

        if (button == MOUSE_SCROLL_DOWN) {
            cameraDistance += cameraDistanceStep;
            zoomFactor = -1.5f;
        }
    }

    void computeTime() {
        GLfloat currentTime = ((GLfloat)glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
        deltaTime = currentTime - oldTime;
        oldTime = currentTime;
    }

    void computeAngleAxis(){
        if (currentX != lastX || currentY != lastY) {
            deltaX = currentX - lastX; 
            deltaY = currentY - lastY; 
            Quaternion rotationQtrnY = Quaternion(deltaX,math::Vector4(0.0f,1.0f,0.0f,1.0f));
            Quaternion rotationQtrnX = Quaternion(deltaY,math::Vector4(1.0f,0.0f,0.0f,1.0f));
            rotationQuaternion = rotationQtrnX * rotationQtrnY * rotationQuaternion;
            lastX = currentX;
            lastY = currentY;
        } else {
            deltaX = 0.0f;
            deltaY = 0.0f;
        }
    }

    void mouseMovement(int x, int y){
        if(KeyBuffer::leftMouseButtonPressed){
            currentX = x;
            currentY = y;
            computeAngleAxis();
        }
    }

    void setupCallbacks() 
    {
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

    void engine_init(int argc, char* argv[]) {
        setupGLUT(argc, argv);
        setupGLEW();
        setupOpenGL();
        setupCallbacks();
    }

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

    float timeSinceStart() {
        return ((float)glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
    }

};

#endif
