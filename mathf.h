#ifndef MATHF_H
#define MATHF_H

#include <cmath>

#include "vector.h"

namespace math {
    #define MAX(x,y) (x > y ? x : y)

    #define MIN(x,y) (x < y ? x : y)

    float floatThreshold = 1.0e-10;

    inline float toRadians(float degrees){
        return (degrees * M_PI) / 180.0f;
    }

    inline float toDegrees(float radians){
        return (radians * 180.0f) / M_PI;
    }

    inline float cotangent(float value){
        float angle = toRadians(value);
        return (1.0f / ((float)tan(angle)));
    }

    inline float compareEqual(float x, float y) {
        float difference = x - y;
        return difference < floatThreshold && -difference < floatThreshold;
    }

    inline float clamp(float n, float minn, float maxn) {
        return MIN(MAX(n, minn), maxn);
    }

    inline void clampVector(Vector3 v, float minn, float maxn) {
        v.x = clamp(v.x, minn, maxn);
        v.y = clamp(v.y, minn, maxn);
        v.z = clamp(v.z, minn, maxn);
    }
};

#endif
