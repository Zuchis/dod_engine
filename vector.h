#ifndef VECTOR_H
#define VECTOR_H

#include <ostream>
#include <array>
#include <cmath>
#include <limits>
#include <iostream>

#include "matrix.h"

namespace math {

    class Matrix2;
    class Matrix3;
    class Matrix4;

    class Vector3{
        private:

        public:
            union {
                struct{
                    float x, y, z;
                };
                std::array<float,3> data;
            };

            Vector3(float x1 = 0.0f, float y1 = 0.0f, float z1 = 0.0f){
                x = x1;
                y = y1;
                z = z1;
            }

            Vector3(const Vector3 &v){
                x = v.x;
                y = v.y;
                z = v.z;
            }

            Vector3 operator + (Vector3 v){
                return Vector3(x + v.x,
                        y + v.y,
                        z + v.z);
            }

            Vector3 operator + (float k){
                return Vector3(x + k,
                        y + k,
                        z + k);
            }

            Vector3 operator - (Vector3 v){
                return Vector3(x - v.x,
                        y - v.y,
                        z - v.z);
            }

            Vector3 operator - () {
                return *this * -1;
            }

            Vector3 operator - (float k){
                return Vector3(x - k,
                        y - k,
                        z - k);
            }

            Vector3 operator / (float k){
                return Vector3(x / k,
                        y / k,
                        z / k);
            }

            Vector3 operator * (float k){
                return Vector3(x * k,
                        y * k,
                        z * k);
            }

            friend Vector3 operator * (Vector3 v, Matrix3 m);

            Vector3 operator = (Vector3 v){
                x = v.x;
                y = v.y;
                z = v.z;
            }

            bool operator == (const Vector3 &u){
                float eps = std::numeric_limits<float>::epsilon();
                float diff1 = x - u.x;
                float diff2 = y - u.y;
                float diff3 = z - u.z;
                return ((diff1 < eps && -diff1 < eps) &&
                        (diff2 < eps && -diff2 < eps) &&
                        (diff3 < eps && -diff3 < eps));
            }

            bool operator != (const Vector3 &u){
                return !(*this == u);
            }

            void operator += (Vector3 v){
                x += v.x;
                y += v.y;
                z += v.z;
            }

            void operator += (float k){
                x += k;
                y += k;
                z += k;
            }

            void operator -= (Vector3 v){
                x -= v.x;
                y -= v.y;
                z -= v.z;
            }

            void operator -= (float k){
                x -= k;
                y -= k;
                z -= k;
            }

            void operator *= (float k){
                x *= k;
                y *= k;
                z *= k;
            }

            void operator /= (float k){
                x /= k;
                y /= k;
                z /= k;
            }

            float norm(){
                return sqrt(x * x + y * y + z * z);
            }

            float quadrance(){
                return (x * x + y * y + z * z);
            }

            float* getData(){
                return data.data();
            }

            Vector3 copy(){
                return Vector3(x, y, z);
            }

            friend Vector3 normalize(Vector3 v);

            Vector3 normalized(){
                Vector3 u = copy();
                return u / u.norm();
            }

            friend float dot (Vector3 u, Vector3 v);

            friend Vector3 cross (Vector3 u, Vector3 v);

            void clean(){
                float small = 1.0e-15f;
                if (x < small && -x < small)
                    x = 0.0;
                if (y < small && -y < small)
                    y = 0.0;
                if (z < small && -z < small)
                    z = 0.0;
            }

            void print(){
                clean();
                std::cout << "( " << x << ", " << y << ", " << z << " )" << std::endl;
            }

            friend std::ostream& operator << (std::ostream& os,Vector3 v){
                v.clean();
                os << "( " << v.x << ", " << v.y << ", " << v.z << " )";
                return os;
            }

            friend Vector3 lerp(Vector3 v, Vector3 u, float t);

    };

    Vector3 normalize(Vector3 v){
        return v / v.norm();
    }

    float dot (Vector3 u, Vector3 v){
        return (u.x * v.x +
                u.y * v.y +
                u.z * v.z);
    }

    Vector3 cross (Vector3 u, Vector3 v){
        return Vector3(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
    }

    // t = 0 -> return v
    Vector3 lerp(Vector3 v, Vector3 u, float t) {
        return Vector3((1.0f - t) * v.x + t * u.x,
                       (1.0f - t) * v.y + t * u.y,
                       (1.0f - t) * v.z + t * u.z);
    }

    // Vector 2D Implementations

    class Vector2{
        private:

        public:
            union {
                struct {
                    float x, y;
                };

                std::array<float,2> data;
            };

            float* getData(){
                return data.data();
            }

            Vector2(float x1 = 0.0f, float y1 = 0.0f){
                x = x1;
                y = y1;
            }

            Vector2(const Vector2 &v){
                x = v.x;
                y = v.y;
            }
            Vector2 operator + (Vector2 v){
                return Vector2(x + v.x,
                        y + v.y);
            }

            Vector2 operator + (float k){
                return Vector2(x + k,
                        y + k);
            }

            Vector2 operator - (Vector2 v){
                return Vector2(x - v.x,
                        y - v.y);
            }

            Vector2 operator - (float k){
                return Vector2(x - k,
                        y - k);
            }

            Vector2 operator / (float k){
                return Vector2(x / k,
                        y / k);
            }

            Vector2 operator * (float k){
                return Vector2(x * k,
                        y * k);
            }

            friend Vector2 operator * (Vector2 v, Matrix2 m);

            Vector2 operator = (Vector2 v){
                x = v.x;
                y = v.y;
            }

            bool operator == (const Vector2 &u){
                float eps = std::numeric_limits<float>::epsilon();
                float diff1 = x - u.x;
                float diff2 = y - u.y;
                return ((diff1 < eps && -diff1 < eps) &&
                        (diff2 < eps && -diff2 < eps));
            }

            bool operator != (const Vector2 &u){
                return !(*this == u);
            }

            void operator += (Vector2 v){
                x += v.x;
                y += v.y;
            }

            void operator += (float k){
                x += k;
                y += k;
            }

            void operator -= (Vector2 v){
                x -= v.x;
                y -= v.y;
            }

            void operator -= (float k){
                x -= k;
                y -= k;
            }

            void operator *= (float k){
                x *= k;
                y *= k;
            }

            void operator /= (float k){
                x /= k;
                y /= k;
            }

            float norm(){
                return sqrt(x * x + y * y);
            }

            float quadrance(){
                return (x * x + y * y);

            }

            Vector2 copy(){
                return Vector2(x, y);
            }

            friend Vector2 normalize(Vector2 v);

            Vector2 normalized(){
                Vector2 u = copy();
                return u / u.norm();
            }

            friend float dot (Vector2 u, Vector2 v);

            void clean(){
                float small = 1.0e-15f;
                if (x < small && -x < small)
                    x = 0.0;
                if (y < small && -y < small)
                    y = 0.0;
            }

            void print(){
                clean();
                std::cout << "( " << x << ", " << y << " )" << std::endl;
            }

            friend std::ostream& operator << (std::ostream& os,Vector2 v){
                v.clean();
                os << "( " << v.x << ", " << v.y  << " )";
                return os;
            }

    };

    Vector2 normalize(Vector2 v){
        return v / v.norm();
    }

    float dot (Vector2 u, Vector2 v){
        return (u.x * v.x +
                u.y * v.y);
    }

    class Vector4{
        private:

        public:
            float x, y, z, w;

            Vector4(float x1 = 0.0f, float y1 = 0.0f, float z1 = 0.0f, float w1 = 0.0f){
                x = x1;
                y = y1;
                z = z1;
                w = w1;
            }

            Vector4(const Vector4 &v){
                x = v.x;
                y = v.y;
                z = v.z;
                w = v.w;
            }

            Vector4(const Vector3 &v){
                x = v.x;
                y = v.y;
                z = v.z;
                w = 1.0f;
            }

            // Vector 4 Implementations

            Vector4 operator + (Vector4 v){
                return Vector4( x + v.x,
                        y + v.y,
                        z + v.z,
                        w + v.w);
            }

            Vector4 operator + (float k){
                return Vector4( x + k,
                        y + k,
                        z + k,
                        w + k);
            }

            Vector4 operator - (Vector4 v){
                return Vector4(x - v.x,
                        y - v.y,
                        z - v.z,
                        w - v.w);
            }

            Vector4 operator - (float k){
                return Vector4(x - k,
                        y - k,
                        z - k,
                        w - k);
            }

            Vector4 operator / (float k){
                return Vector4(x / k,
                        y / k,
                        z / k,
                        w / k);
            }

            Vector4 operator * (float k){
                return Vector4(x * k,
                        y * k,
                        z * k,
                        w * k);
            }

            friend Vector4 operator * (Vector4 v, Matrix4 m);

            Vector4 operator = (Vector4 v){
                x = v.x;
                y = v.y;
                z = v.z;
                w = v.w;
            }

            bool operator == (const Vector4 &u){
                float eps = std::numeric_limits<float>::epsilon();
                float diff1 = x - u.x;
                float diff2 = y - u.y;
                float diff3 = z - u.z;
                float diff4 = w - u.w;
                return ((diff1 < eps && -diff1 < eps) &&
                        (diff2 < eps && -diff2 < eps) &&
                        (diff3 < eps && -diff3 < eps) && 
                        (diff4 < eps && -diff4 < eps));
            }

            bool operator != (const Vector4 &u){
                return !(*this == u);
            }

            void operator += (Vector4 v){
                x += v.x;
                y += v.y;
                z += v.z;
                w += v.w;
            }

            void operator += (float k){
                x += k;
                y += k;
                z += k;
                w += k;
            }

            void operator -= (Vector4 v){
                x -= v.x;
                y -= v.y;
                z -= v.z;
                w -= v.w;
            }

            void operator -= (float k){
                x -= k;
                y -= k;
                z -= k;
                w -= k;
            }

            void operator *= (float k){
                x *= k;
                y *= k;
                z *= k;
                w *= k;
            }

            void operator /= (float k){
                x /= k;
                y /= k;
                z /= k;
                w /= k;
            }

            float norm(){
                return sqrt(x * x + y * y + z * z + w * w);
            }

            float quadrance(){
                return (x * x + y * y + z * z + w * w);

            }

            Vector4 copy(){
                return Vector4(x, y, z, w);
            }

            friend Vector4 normalize(Vector4 u);

            Vector4 normalized(){
                Vector4 u = copy();
                return u / u.norm();
            }

            friend float dot (Vector4 u, Vector4 v);

            void clean(){
                float small = 1.0e-15f;
                if (x < small && -x < small)
                    x = 0.0;
                if (y < small && -y < small)
                    y = 0.0;
                if (z < small && -z < small)
                    z = 0.0;
                if (w < small && -w < small)
                    w = 0.0;
            }

            void print(){
                clean();
                std::cout << "( " << x << ", " << y << ", " << z << ", " << w << " )" << std::endl;
            }

            friend std::ostream& operator << (std::ostream& os,Vector4 v){
                v.clean();
                os << "( " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " )";
                return os;
            }

    };
    
    Vector4 normalize(Vector4 u){
        return u / u.norm();
    }
    
    float dot (Vector4 u, Vector4 v){
        return (u.x * v.x +
                u.y * v.y +
                u.z * v.z +
                u.w * v.w);
    }

};

#endif
