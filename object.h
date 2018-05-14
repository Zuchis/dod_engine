#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "mesh.h"
#include "matrixfactory.h"
#include "quaternion.h"

using std::vector;

using math::Quaternion;
using math::Matrix4;


namespace engine {

    struct ObjectModels {
        Vector3* translations;
        Quaternion* rotations;
        Vector3* scales;
    };

    struct ObjectLabels {
       std::string* labels;
    };

    struct ObjectPhysics {
        Vector3* speeds;
        Vector3* accelerations;
    };

    class Object {
        public: 
            Vector3 translation;
            Quaternion rotation;
            Vector3 scale;

            std::string label;

            Vector3 speed;
            Vector3 acceleration;

            float reflectionFactor = 1.0f;

            Object(std::string _label = "unamed") {
                translation = Vector3();
                rotation = Quaternion(1.0f,0.0f,0.0f,0.0f);
                scale = Vector3(1.0f, 1.0f, 1.0f);

                label = _label;

                speed = Vector3();
                acceleration = Vector3();
            }

            Matrix4 modelMatrix() {
                return (
                        math::translate(translation) *
                        rotation.toMatrix() *
                        math::scale(scale)
                       );
            }

    };
};
