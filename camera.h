#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include <GL/glew.h>

#include "matrixfactory.h"

using math::Vector3;
using math::Matrix4;

#define  YAW        -90.00
#define  PITCH        -55.00
#define  SPEED        3.00
#define  SENSITIVITY  0.25
#define  ZOOM         30.0

enum Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

enum WheelMovement {
    MOUSE_SCROLL_UP = 3,
    MOUSE_SCROLL_DOWN = 4
};

class ArcballCamera {
    public:
        GLuint UBO_BP;
        GLuint matricesVbo;

        ArcballCamera(GLuint _UBO_BP) {
            UBO_BP = _UBO_BP;
            glGenBuffers(1,&matricesVbo);
            glBindBuffer(GL_UNIFORM_BUFFER,matricesVbo);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(std::array<float,16>) * 2, 0, GL_STREAM_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, matricesVbo);
        }

        void setViewMatrix(Matrix4 viewMatrix) {
            glBindBuffer(GL_UNIFORM_BUFFER, matricesVbo);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(viewMatrix.data), viewMatrix.getData());
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        void setProjectionMatrix(Matrix4 projectionMatrix) {
            glBindBuffer(GL_UNIFORM_BUFFER, matricesVbo);
            float* matrix = projectionMatrix.getData();
            glBufferSubData(GL_UNIFORM_BUFFER, sizeof(projectionMatrix.data), sizeof(projectionMatrix.data), matrix);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
};

#endif
