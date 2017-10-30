#ifndef QUATERNIONS_H
#define QUATERNIONS_H

#include <iostream>
#include <iomanip>
#include <string>

#include "matrixfactory.h"

namespace math {

    class Quaternion{
        private:
            const float qThreshold = 1.0e-5f;
        public:
            union{
                struct {
                    float t, x, y, z;
                };
                std::array<float,4> data;
            };

            Quaternion(float _t = 0.0f, float _x = 0.0f, float _y = 0.0f, float _z = 0.0f){
                t = _t;
                x = _x;
                y = _y;
                z = _z;
            }

            Quaternion(float theta, Vector4 axis){
                Vector4 axisn = math::normalize(axis);

                float a = math::toRadians(theta);
                t = cos(a / 2.0f);
                float s = sin(a / 2.0f);
                x = axisn.x * s;
                y = axisn.y * s;
                z = axisn.z * s;
                clean();
                normalize();
            }

            float theta() { 
                normalize();
                float qtrnAngle = 2.0f * acos(t);
                return math::toDegrees(qtrnAngle);
            }

            Vector4 axis() {
                normalize();
                float s = sqrt(1 - t*t);
                if (s < qThreshold)
                    return Vector4(1.0f,0.0f,0.0f,1.0f);

                return Vector4(x / s, y / s, z / s, 1.0f);
            }

            void clean () {
                int i;
                for (i = 0; i < 4; i++) {
                    if(fabs(data[i]) < qThreshold)
                        data[i] = 0.0f;
                }
            }

            float quadrance() {return t * t + x * x + y * y + z * z;}

            float norm() {return sqrt(quadrance());}

            void normalize() {
                float s = 1 / norm();
                *this = *this * s;
            }

            Quaternion& operator= (Quaternion q) {
                t = q.t;
                x = q.x;
                y = q.y;
                z = q.z;
                return *this;
            }

            Quaternion operator+ (Quaternion q) {
                return Quaternion(t + q.t,
                                  x + q.x,
                                  y + q.y,
                                  z + q.z);
            }

            Quaternion operator* (float k) {
                return Quaternion(t * k,
                                  x * k,
                                  y * k,
                                  z * k);
            }

            Quaternion operator* (Quaternion q) {
                return Quaternion(t * q.t - x * q.x - y * q.y - z * q.z,
                                  t * q.x + x * q.t + y * q.z - z * q.y,
                                  t * q.y + y * q.t + z * q.x - x * q.z,
                                  t * q.z + z * q.t + x * q.y - y * q.x);
            }

            bool operator== (Quaternion q) {
                return (fabs(t - q.t) < qThreshold &&
                        fabs(x - q.x) < qThreshold &&
                        fabs(y - q.y) < qThreshold &&
                        fabs(z - q.z) < qThreshold);
            }

            Quaternion conjugate() {return Quaternion(t, -x, -y, -z);}

            Quaternion inverse() { return conjugate() * (1.0f / quadrance());}

            friend Quaternion normalize(Quaternion q);

            Matrix4 toMatrix() {
                 Matrix4 m4(1 - 2*y*y - 2*z*z,   2*x*y - 2*t*z  ,   2*x*z + 2*t*y  , 0,
                              2*x*y + 2*t*z  , 1 - 2*x*x - 2*z*z,   2*y*z - 2*t*x  , 0,
                              2*x*z - 2*t*y  ,   2*y*z + 2*t*x  , 1 - 2*x*x - 2*y*y, 0,
                                    0        ,         0        ,        0         , 1);
                 m4.clean();
                 return m4;
            }

            friend Quaternion lerp(Quaternion q0, Quaternion q1, float k);

            friend Quaternion slerp(Quaternion q0, Quaternion q1, float k);

            friend std::ostream& operator << (std::ostream& os, Quaternion q) {
                os << "(" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")";
                return os;
            }

            void printAngleAxis(){
                std::cout << "[" << theta() << ", " << axis() << "]" << std::endl;
            }

    };

    Quaternion normalize(Quaternion q){
        float s = 1 / q.norm();
        return q * s;
    }

    Quaternion lerp(Quaternion q0, Quaternion q1, float k){
        float cos_angle = q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t;
        float k0 = 1.0f - k;
        float k1 = (cos_angle > 0) ? k : -k;
        Quaternion qi = (q0 * k0) + (q1 * k1);
        qi.normalize();
        return qi;
    }

    Quaternion slerp(Quaternion q0, Quaternion q1, float k){
        float angle = acos(q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t);
        float k0 = sin((1-k)*angle) / sin(angle);
        float k1 = sin(k*angle) / sin(angle);
        Quaternion qi = (q0 * k0) + (q1 * k1);
        qi.normalize();
        return qi;
    }
};

#endif
