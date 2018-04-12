#pragma once

#include "object.h"
#include "mathf.h"

#define MIN_SPEED -2
#define MAX_SPEED 2

using engine::Quaternion;

class Sphere : public engine::Object {
    public:
        Sphere(std::string _label = "unamed")
            :Object(_label) {}

        virtual void update() {
            math::clampVector(speed + acceleration, (float)MIN_SPEED, (float)MAX_SPEED);

            translation = translation + speed;

            Quaternion rotationQtrnY = Quaternion(speed.x * 100, math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
            Quaternion rotationQtrnX = Quaternion(-speed.y * 100, math::Vector4(1.0f, 0.0f, 0.0f, 1.0f));

            rotation = rotationQtrnX * rotationQtrnY * rotation;
        }
};

class Parallelogram : public engine::Object {
    public:
        Parallelogram(std::string _label = "unamed")
            :Object(_label) {}

        virtual void update() {
            math::clampVector(speed + acceleration, (float)MIN_SPEED, (float)MAX_SPEED);

            translation = translation + speed;

            Quaternion rotationQtrnX = Quaternion(-speed.y * 100, math::Vector4(1.0f, 0.0f, 0.0f, 1.0f));

            rotation = rotationQtrnX * rotation;
        }
};

class Suzanne : public engine::Object {
    public:
        Suzanne(std::string _label = "unamed")
            :Object(_label) {}

        virtual void update() {
            math::clampVector(speed + acceleration, (float)MIN_SPEED, (float)MAX_SPEED);

            translation = translation + speed;

            Quaternion rotationQtrnY = Quaternion(speed.x * 100, math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
            Quaternion rotationQtrnX = Quaternion(-speed.y * 100, math::Vector4(1.0f, 0.0f, 0.0f, 1.0f));

            rotation = rotationQtrnX * rotationQtrnY;
        }
};

class Plane : public engine::Object {
    public:
        Plane(std::string _label = "unamed")
            :Object(_label) {}

        virtual void update() {
            math::clampVector(speed + acceleration, (float)MIN_SPEED, (float)MAX_SPEED);

            translation = translation + speed;
        }
};
