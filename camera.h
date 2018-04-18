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

class ICamera {
    public:
        GLuint UBO_BP;
        GLuint matricesVbo;
        ICamera(GLuint _UBO_BP) {
            UBO_BP = _UBO_BP;
            glGenBuffers(1,&matricesVbo);
            glBindBuffer(GL_UNIFORM_BUFFER,matricesVbo);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(std::array<float,16>) * 2, 0, GL_STREAM_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, matricesVbo);
        }

        virtual void setViewMatrix(Matrix4 viewMatrix) = 0;
        virtual void setProjectionMatrix(Matrix4 projectionMatrix) = 0;
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

class FPSCamera : public ICamera {
    private:
        void updateCameraVectors(){
            Vector3 _front = Vector3();
            _front.x = cos(math::toRadians(yaw)) * cos(math::toRadians(pitch));
            _front.y = sin(math::toRadians(pitch));
            _front.z = sin(math::toRadians(yaw)) * cos(math::toRadians(pitch));
            this->front = _front.normalized();
            this->right = (math::cross(this->front,this->worldUp)).normalized();
            this->up    = (math::cross(this->right,this->front)).normalized();
        }
    public:
        Vector3 position;
        Vector3 front;
        Vector3 up;
        Vector3 right;
        Vector3 worldUp;

        GLfloat yaw;
        GLfloat pitch;
        GLfloat movementSpeed;
        GLfloat mouseSensitivity;
        GLfloat zoom;

        FPSCamera(GLuint _UBO_BP, Vector3 _position = Vector3(0.0f,0.0f,3.0f), Vector3 _up = Vector3(0.0f,1.0f,0.0f), GLfloat _yaw = (GLfloat)YAW, GLfloat _pitch = (GLfloat)PITCH) 
            : ICamera(_UBO_BP), front(Vector3(0.0f,0.0f,-1.0f)), movementSpeed((GLfloat)SPEED), mouseSensitivity((GLfloat)SENSITIVITY), zoom((GLfloat)ZOOM) {
            position = _position;
            worldUp = _up;
            yaw = _yaw;
            pitch = _pitch;
            updateCameraVectors();
        }

        virtual void setViewMatrix(Matrix4 viewMatrix) {
            glBindBuffer(GL_UNIFORM_BUFFER, matricesVbo);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(viewMatrix.data), viewMatrix.getData());
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        virtual void setProjectionMatrix(Matrix4 projectionMatrix) {
            glBindBuffer(GL_UNIFORM_BUFFER, matricesVbo);
            float* matrix = projectionMatrix.getData();
            glBufferSubData(GL_UNIFORM_BUFFER, sizeof(projectionMatrix.data), sizeof(projectionMatrix.data), matrix);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        Matrix4 getViewMatrix(){
            return lookAt(this->position, this->position + this->front, this->up);
        }

        Matrix4 lookAt(Vector3 eye, Vector3 center, Vector3 up){
            Vector3 view = center - eye;
            Vector3 v = view.normalized();
            Vector3 side = math::cross(v,up);
            Vector3 s = side.normalized();
            Vector3 u = math::cross(s,v);

            Matrix4 viewMatrix = Matrix4(s.x, s.y, s.z, -math::dot(s,eye),
                                         u.x, u.y, u.z, -math::dot(u,eye),
                                        -v.x,-v.y, -v.z, math::dot(v,eye),
                                         0, 0, 0 ,1);

            return viewMatrix;
        }

        void computeKeyboardInputs(Movement direction, GLfloat deltaTime)
        {
            GLfloat velocity = this->movementSpeed * deltaTime;

            if (direction == FORWARD)
                this->position += this->front * velocity;
            if (direction == BACKWARD)
                this->position -= this->front * velocity;
            if (direction == LEFT)
                this->position -= this->right * velocity;
            if (direction == RIGHT)
                this->position += this->right * velocity;
        }

        void computeMouseMovement(GLfloat xoffset, GLfloat yoffset){
        
            bool constrainPitch = true; // HACK
            xoffset *= this->mouseSensitivity;
            yoffset *= this->mouseSensitivity;

            this->yaw   += xoffset;
            this->pitch += yoffset;

            if (constrainPitch)
            {
                if (this->pitch > 89.0f)
                    this->pitch = 89.0f;
                if (this->pitch < -89.0f)
                    this->pitch = -89.0f;
            }

            updateCameraVectors();
        }

        void computeMouseScroll(GLfloat yoffset)
        {
            if (this->zoom >= 1.0f && this->zoom <= 45.0f)
                this->zoom -= yoffset;
            if (this->zoom <= 1.0f)
                this->zoom = 1.0f;
            if (this->zoom >= 45.0f)
                this->zoom = 45.0f;
        }
};
#endif
