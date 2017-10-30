#pragma once

#include <SFML/Graphics.hpp>
#include <GL/glew.h>

#include "math/math.h"
#include "visual/visual.h"

namespace engine {
    const unsigned int VCOORDS = 0;
    const unsigned int TCOORDS = 1;
    const unsigned int NCOORDS = 2;

    GLuint UBO_BP = 0;

    float cameraDistance = 15;

    void engine_init(void);
};
