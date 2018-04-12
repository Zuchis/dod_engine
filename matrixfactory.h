#ifndef MATRIXFACTORY_H
#define MATRIXFACTORY_H

#include "matrix.h"
#include "mathf.h"

namespace math{

    const Vector3 x_axis = Vector3(1.0f,0.0f,0.0f);
    const Vector3 y_axis = Vector3(0.0f,1.0f,0.0f);
    const Vector3 z_axis = Vector3(0.0f,0.0f,1.0f);

    Matrix2 Create2DIdentity(){
        Matrix2 identity = Matrix2(1.0f,0.0f,0.0f,1.0f);
        return identity;
    }

    Matrix3 Create3DIdentity(){
        Matrix3 identity = Matrix3(1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,1.0f);
        return identity;
    }

    Matrix4 Create4DIdentity(){
        Matrix4 identity = Matrix4(1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f);
        return identity;
    }

    Matrix2 Create2DRotation(float angle){
        float radToDegree = (M_PI * angle) / 180.0f;
        float sin0 = sin(radToDegree);
        float cos0 = cos(radToDegree);
        Matrix2 rot = Matrix2(cos0,sin0 * -1, sin0, cos0);
        return rot;
    }

    Matrix3 Create3DZRotation(float angle){
        float radToDegree = (M_PI * angle) / 180.0f;
        float sin0 = sin(radToDegree);
        float cos0 = cos(radToDegree);
        Matrix3 rot = Matrix3(cos0,sin0 * -1,0.0f,sin0,cos0,0.0f,0.0f,0.0f,1.0f);
        return rot;
    }

    Matrix3 Create3DYRotation(float angle){
        float radToDegree = (M_PI * angle) / 180.0f;
        float sin0 = sin(radToDegree);
        float cos0 = cos(radToDegree);
        Matrix3 rot = Matrix3(cos0,0.0f,sin0,0.0f,1.0f,0.0f,sin0 * -1,0.0f,cos0);
        return rot;
    }

    Matrix3 Create3DXRotation(float angle){
        float radToDegree = (M_PI * angle) / 180.0f;
        float sin0 = sin(radToDegree);
        float cos0 = cos(radToDegree);
        Matrix3 rot = Matrix3(1.0f,0.0f,0.0f,0.0f,cos0,sin0 * -1,0.0f,sin0,cos0);
        return rot;
    }

    Matrix3 Create3DTranslation(float XTranslation, float YTranslation){
        Matrix3 trans = Matrix3(1.0f,0.0f,XTranslation,0.0f,1.0f,YTranslation,0.0f,0.0f,1.0f);
        return trans;
    }

    Matrix4 Create4DTranslation(float XTranslation, float YTranslation, float ZTranslation){
        Matrix4 trans = Matrix4(1.0f,0.0f,0.0f,XTranslation,0.0f,1.0f,0.0f,YTranslation,0.0f,0.0f,1.0f,ZTranslation,0.0f,0.0f,0.0f,1.0f);
        return trans;
    }

    Matrix2 Create2DScalingMatrix(float XFactor, float YFactor){
        Matrix2 scale = Matrix2(XFactor,0.0f,0.0f,YFactor);
        return scale;
    }

    Matrix3 Create3DScalingMatrix(float XFactor, float YFactor, float ZFactor){
        Matrix3 scale = Matrix3(XFactor,0.0f,0.0f,0.0f,YFactor,0.0f,0.0f,0.0f,ZFactor);
        return scale;
    }

    Matrix3 CONVERT_4D_TO_3D_MATRIX(Matrix4 m){
        int i,j;
        int height = 3;
        int width  = 3;
        Matrix3 converted = Matrix3();
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                converted.setElement(i,j,m.getElement(i,j));
            }
        }
        return converted;
    }

    Matrix2 CONVERT_3D_TO_2D_MATRIX(Matrix3 m){
        int i,j;
        int height = 2;
        int width  = 2;
        Matrix2 converted = Matrix2();
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                converted.setElement(i,j,m.getElement(i,j));
            }
        }
        return converted;
    }

    Matrix3 CONVERT_2D_TO_3D_MATRIX(Matrix2 m){
        int i,j;
        int height = 2;
        int width  = 2;
        Matrix3 converted = Matrix3();
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                converted.setElement(i,j,m.getElement(i,j));
            }
        }
        converted.setElement(2,2,1.0f);
        return converted;
    }

    Matrix4 CONVERT_3D_TO_4D_MATRIX(Matrix3 m){
        int i,j;
        int height = 3;
        int width  = 3;
        Matrix4 converted = Matrix4();
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                converted.setElement(i,j,m.getElement(i,j));
            }
        }
        converted.setElement(3,3,1.0f);
        return converted;
    }

    Matrix3 Create3DDualMatrix(Vector3 a){
        Matrix3 dual = Matrix3(0.0f,a.z * -1,a.y,a.z,0.0f,a.x * -1,a.y * -1,a.x,0.0f);
        return dual;
    }

    Matrix3 CreateAxisRotation(float angle,Vector3 axis){
        float degAngle = (angle * M_PI) / 180.0f;
        float sin0 = sin(degAngle);
        float cos0 = cos(degAngle);
        Matrix3 Id = Create3DIdentity();
        Matrix3 dualv0 = Create3DDualMatrix(axis.normalized());
        Matrix3 Rv0 = Id + (sin0 * dualv0) + ((1 - cos0) * dualv0 * dualv0);
        return Rv0;
    }

    Matrix4 CreateTransformMatrix(float xTranslation,float yTranslation,float zTranslation,float angle, float scalingFactor){
        //Matrix4 id = Create4DIdentity();
        Matrix4 translation = Create4DTranslation(xTranslation,yTranslation,zTranslation);
        Matrix3 _rotation = Create3DZRotation(angle);
        Matrix4 rotation = CONVERT_3D_TO_4D_MATRIX(_rotation);
        Matrix3 _scaling = Create3DScalingMatrix(scalingFactor,scalingFactor,scalingFactor);
        Matrix4 scaling = CONVERT_3D_TO_4D_MATRIX(_scaling);
        Matrix4 transform = translation * rotation * scaling;
        return transform;
    }

    Matrix4 Perspective(float fovy, float aspect, float zNear, float zFar) {
        float f = math::cotangent(fovy / 2.0f);
        return Matrix4(f / aspect,  0,                0                ,                  0,
                            0     ,  f,                0                ,                  0,
                            0     ,  0, (zFar + zNear) / (zNear - zFar) , (2 * zFar * zNear) / (zNear - zFar),
                            0     ,  0,               -1                ,                  0);
    }

    Matrix4 Ortho(float left, float right, float bottom, float top, float near, float far){
        return Matrix4(2.0f / (right - left),        0             ,         0          , (left + right) / (left - right),
                                0           , 2.0f / (top - bottom),         0          , (bottom + top) / (bottom - top),
                                0           ,        0             , -2.0f / (far - near),   (near + far) / (near - far)  ,
                                0           ,        0             ,         0          ,                1               );
    }

    Matrix4 translate(Vector3 v) {
        Matrix4 translationMatrix = Create4DTranslation(v.x,v.y,v.z);
        return translationMatrix;
    }

    Matrix4 rotate(float angle, Vector3 axis) {
        Matrix4 rotationMatrix = CONVERT_3D_TO_4D_MATRIX(CreateAxisRotation(angle,axis));
        return rotationMatrix;
    }

    Matrix4 scale(Vector3 v) {
        Matrix4 scaleMatrix = CONVERT_3D_TO_4D_MATRIX(Create3DScalingMatrix(v.x,v.y,v.z));
        return scaleMatrix;
    }

    // HACK
    Vector2 operator * (Vector2 v, Matrix2 m){
        Vector2 mul = Vector2();
        mul.x = m.getElement(0,0) * v.x + m.getElement(1,0) * v.y;
        mul.y = m.getElement(0,1) * v.x + m.getElement(1,1) * v.y;
        return mul;    
    }

    Vector3 operator * (Vector3 v, Matrix3 m){
        Vector3 mul = Vector3();
        mul.x = m.getElement(0,0) * v.x + m.getElement(1,0) * v.y + m.getElement(2,0) * v.z;
        mul.y = m.getElement(0,1) * v.x + m.getElement(1,1) * v.y + m.getElement(2,1) * v.z;
        mul.z = m.getElement(0,2) * v.x + m.getElement(1,2) * v.y + m.getElement(2,2) * v.z;
        return mul;    
    }

    Vector4 operator * (Vector4 v, Matrix4 m){
        Vector4 mul = Vector4();
        mul.x = m.getElement(0,0) * v.x + m.getElement(1,0) * v.y + m.getElement(2,0) * v.z + m.getElement(3,0) * v.w;
        mul.y = m.getElement(0,1) * v.x + m.getElement(1,1) * v.y + m.getElement(2,1) * v.z + m.getElement(3,1) * v.w;
        mul.z = m.getElement(0,2) * v.x + m.getElement(1,2) * v.y + m.getElement(2,2) * v.z + m.getElement(3,2) * v.w;
        mul.w = m.getElement(0,3) * v.x + m.getElement(1,3) * v.y + m.getElement(2,3) * v.z + m.getElement(3,3) * v.w;
        return mul;    
    }
}
#endif
