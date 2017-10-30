#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include "vector.h"
#include <iomanip>

namespace math {

    class Vector3;
    class Vector2;
    class Vector4;

    class Matrix2{
        private:
            int width = 2;
            int height = 2;
            int getPosition(int i,int j) const {return (i + j * width);}

        public:

            std::array<float,4> data;

            Matrix2(){
                int i, j;
                for(i = 0; i < height; i++){
                    for(j = 0; j < width; j++){
                        data[i + j * width] = 0.0;
                    }
                }
            }

            Matrix2(const Matrix2 &m){
                int i, j, index;
                for(i = 0; i < height; i++){
                    for(j = 0; j < width; j++){
                        index = i + j * width;
                        data[index] = m.getElement(i,j);
                    }
                }
            }

            Matrix2(float values[4]){
                int i, j, index;
                for(i = 0; i < height; i++){
                    for(j = 0; j < width; j++){
                        index = i + j * width;
                        data[index] = values[j + i * height];
                    }
                }
            }

            Matrix2(std::vector<float> values){
                int i, j, index;
                for(i = 0; i < height; i++){
                    for(j = 0; j < width; j++){
                        index = i + j * width;
                        data[index] = values[j + i * height];
                    }
                }
            }

            Matrix2(float a00, float a01,
                    float a10, float a11){
                data[0] = a00;
                data[1] = a10;
                data[2] = a01;
                data[3] = a11;
            }
            float getElement(int i,int j) const {
                int index = getPosition(i, j);
                return data[index];
            }

            void setElement(int i,int j,float value){
                int index = getPosition(i, j);
                data[index] = value;
                return;
            }

            void setElement(int index, float value){
                data[index] = value;
                return;
            }

            Matrix2 operator = (Matrix2 m){
                int i,j,index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        data[index] = m.getElement(i,j);
                    }
                }
            }

            Matrix2 copy(){
                Matrix2 copypasta = Matrix2();
                int i,j,index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        copypasta.setElement(index,data[index]);
                    }
                }
                return copypasta;
            }

            Matrix2 translated(){
                Matrix2 trans = Matrix2();
                int i,j,index;
                for (j = 0; j < height; j++) {
                    for (i = 0; i < width; i++) {
                        index = getPosition(i,j);
                        trans.setElement(i,j,data[index]);
                    }
                }
            }

            void print(){
                std::cout << "|";
                int i,j;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        if(j != width -1)
                            std::cout << getElement(i,j) << ", ";
                        else
                            std::cout << getElement(i,j) << "|" << std::endl;
                    }
                    if(i != height -1)
                        std::cout << "|";
                }
            }

            Matrix2 operator / (float k) {
                Matrix2 m = Matrix2();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] / k);
                    }
                }
                return m;
            }

            Matrix2 operator * (float k) {
                Matrix2 m = Matrix2();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] * k);
                    }
                }
                return m;
            }

            friend Matrix2 operator * (float k, const Matrix2 &m1) {
                int height = 2;
                int width =  2;
                Matrix2 m = Matrix2();
                int i, j;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        m.setElement(i,j,m1.getElement(i,j) * k);
                    }
                }
                return m;
            }

            Matrix2 operator + (float k) {
                Matrix2 m = Matrix2();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] + k);
                    }
                }
                return m;
            }

            Matrix2 operator - (float k) {
                Matrix2 m = Matrix2();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] - k);
                    }
                }
                return m;
            }

            Matrix2 operator + (Matrix2 m2) {
                Matrix2 m = Matrix2();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] + m2.getElement(i,j));
                    }
                }
                return m;
            }

            Matrix2 operator - (Matrix2 m2) {
                Matrix2 m = Matrix2();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] - m2.getElement(i,j));
                    }
                }
                return m;
            }

            float determinant(){
                float a = getElement(0,0);
                float b = getElement(0,1);
                float c = getElement(1,0);
                float d = getElement(1,1);
                return (a * d - b * c);
            }

            Matrix2 inverted(){
                Matrix2 inv = Matrix2();
                inv.setElement(0,0,getElement(1,1));
                inv.setElement(0,1,getElement(0,1) * -1);
                inv.setElement(1,0,getElement(1,0) * -1);
                inv.setElement(1,1,getElement(0,0));
                return inv / determinant();
            }

            std::vector<float> row(int r){
                std::vector<float> elements;
                int index, j;
                for (j = 0; j < width; j++) {
                    index = getPosition(r,j);
                    elements.insert(elements.end(),data[index]);
                }
                return elements;
            }

            std::vector<float> column(int col){
                std::vector<float> elements;
                int i, index;
                for (i = 0; i < height; i++) {
                    index = getPosition(i,col);
                    elements.insert(elements.end(),data[index]);
                }
                return elements;
            }


            Matrix2 operator * (Matrix2 m){
                Matrix2 mul = Matrix2();
                int i,j,k,index;
                float currentValue = 0.0f;
                std::vector<float> r, c;
                std::vector<float>::size_type size_;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        r = row(i);
                        c = m.column(j);
                        size_ = c.size();
                        for (k = 0; k < (int)size_; k++) {
                            currentValue += r[k] * c[k];
                        }
                        index = getPosition(i,j);
                        mul.setElement(index,currentValue);
                        currentValue = 0.0;
                    }
                }
                return mul;
            }

            Vector2 operator * (Vector2 v){
                Vector2 mul = Vector2();
                mul.x = getElement(0,0) * v.x + getElement(0,1) * v.y;
                mul.y = getElement(1,0) * v.x + getElement(1,1) * v.y;
                return mul;    
            }

            bool operator == (const Matrix2 &m){
                float eps = std::numeric_limits<float>::epsilon();
                int i,j,index;
                float diff;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        diff = data[index] - m.getElement(i,j);
                        if(diff < eps && -diff < eps)
                            continue;
                        else
                            return false;
                    }
                }
                return true;
            }

            bool operator != (const Matrix2 &m){
                return !(*this == m);
            }

    };

    class Matrix3{
        private:
            int width  = 3;
            int height = 3;
            int getPosition(int i,int j) const {return (i + j * width);}

        public:
            std::array<float,9> data;
            Matrix3(){
                int i, j;
                for(i = 0; i < height; i++){
                    for(j = 0; j < width; j++){
                        data[i + j * width] = 0.0;
                    }
                }
            }

            Matrix3(const Matrix3 &m){
                int i, j, index;
                for(i = 0; i < height; i++){
                    for(j = 0; j < width; j++){
                        index = i + j * width;
                        data[index] = m.getElement(i,j);
                    }
                }
            }

            Matrix3(float values[9]){
                int i, j, index;
                for(i = 0; i < height; i++){
                    for(j = 0; j < width; j++){
                        index = i + j * width;
                        data[index] = values[j + i * height];
                    }
                }
            }

            Matrix3(std::vector<float> values){
                int i, j, index;
                for(i = 0; i < height; i++){
                    for(j = 0; j < width; j++){
                        index = i + j * width;
                        data[index] = values[j + i * height];
                    }
                }
            }

            Matrix3(float a00, float a01, float a02,
                    float a10, float a11, float a12,
                    float a20, float a21, float a22){
                data[0] = a00;
                data[1] = a10;
                data[2] = a20;
                data[3] = a01;
                data[4] = a11;
                data[5] = a21;
                data[6] = a02;
                data[7] = a12;
                data[8] = a22;
            }
            // Matrix 3D Implementations

            float getElement(int i,int j) const {
                int index = getPosition(i, j);
                return data[index];
            }

            void setElement(int i,int j,float value){
                int index = getPosition(i, j);
                data[index] = value;
                return;
            }

            void setElement(int index, float value){
                data[index] = value;
                return;
            }

            Matrix3 operator = (Matrix3 m){
                int i,j,index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        data[index] = m.getElement(i,j);
                    }
                }
            }

            Matrix3 copy(){
                Matrix3 copypasta = Matrix3();
                int i,j,index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        copypasta.setElement(index,data[index]);
                    }
                }
                return copypasta;
            }

            Matrix3 translated(){
                Matrix3 trans = Matrix3();
                int i,j,index;
                for (j = 0; j < height; j++) {
                    for (i = 0; i < width; i++) {
                        index = getPosition(i,j);
                        trans.setElement(i,j,data[index]);
                    }
                }
                return trans;
            }

            void clean(){
                float eps = 1.0e-5f;
                int i,j,index;
                float diff;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        diff = data[index];
                        if(diff < eps && -diff < eps)
                            data[index] = 0.0;;
                    }
                }
            }

            void print(){
                clean();
                std::cout << "|";
                int i,j;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        if(j != width -1)
                            std::cout << getElement(i,j) << ", ";
                        else
                            std::cout << getElement(i,j) << "|" << std::endl;
                    }
                    if(i != height -1)
                        std::cout << "|";
                }
            }

            Matrix3 operator / (float k) {
                Matrix3 m = Matrix3();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] / k);
                    }
                }
                return m;
            }

            Matrix3 operator * (float k) {
                Matrix3 m = Matrix3();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] * k);
                    }
                }
                return m;
            }

            friend Matrix3 operator * (float k, const Matrix3 &m1) {
                int height = 3;
                int width =  3;
                Matrix3 m = Matrix3();
                int i, j;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        m.setElement(i,j,m1.getElement(i,j) * k);
                    }
                }
                return m;
            }


            Matrix3 operator + (float k) {
                Matrix3 m = Matrix3();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] + k);
                    }
                }
                return m;
            }

            Matrix3 operator - (float k) {
                Matrix3 m = Matrix3();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] - k);
                    }
                }
                return m;
            }

            Matrix3 operator + (Matrix3 m2) {
                Matrix3 m = Matrix3();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] + m2.getElement(i,j));
                    }
                }
                return m;
            }

            Matrix3 operator - (Matrix3 m2) {
                Matrix3 m = Matrix3();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] - m2.getElement(i,j));
                    }
                }
                return m;
            }

            float determinant(){
                float det = getElement(0, 0) * (getElement(1, 1) * getElement(2, 2) - getElement(2, 1) * getElement(1, 2)) -
                    getElement(0, 1) * (getElement(1, 0) * getElement(2, 2) - getElement(1, 2) * getElement(2, 0)) +
                    getElement(0, 2) * (getElement(1, 0) * getElement(2, 1) - getElement(1, 1) * getElement(2, 0));
                return det;
            }

            Matrix3 inverted(){
                float det = determinant();
                float inverseDeterminant = 1 / det;

                Matrix3 inverse = Matrix3();
                inverse.setElement(0, 0,((getElement(1, 1) * getElement(2, 2) - getElement(2, 1) * getElement(1, 2)) * inverseDeterminant));
                inverse.setElement(0, 1,((getElement(0, 2) * getElement(2, 1) - getElement(0, 1) * getElement(2, 2)) * inverseDeterminant));
                inverse.setElement(0, 2,((getElement(0, 1) * getElement(1, 2) - getElement(0, 2) * getElement(1, 1)) * inverseDeterminant));
                inverse.setElement(1, 0,((getElement(1, 2) * getElement(2, 0) - getElement(1, 0) * getElement(2, 2)) * inverseDeterminant));
                inverse.setElement(1, 1,((getElement(0, 0) * getElement(2, 2) - getElement(0, 2) * getElement(2, 0)) * inverseDeterminant));
                inverse.setElement(1, 2,((getElement(1, 0) * getElement(0, 2) - getElement(0, 0) * getElement(1, 2)) * inverseDeterminant));
                inverse.setElement(2, 0,((getElement(1, 0) * getElement(2, 1) - getElement(2, 0) * getElement(1, 1)) * inverseDeterminant));
                inverse.setElement(2, 1,((getElement(2, 0) * getElement(0, 1) - getElement(0, 0) * getElement(2, 1)) * inverseDeterminant));
                inverse.setElement(2, 2,((getElement(0, 0) * getElement(1, 1) - getElement(1, 0) * getElement(0, 1)) * inverseDeterminant));

                return inverse;
            }

            std::vector<float> row(int r){
                std::vector<float> elements;
                int index, j;
                for (j = 0; j < width; j++) {
                    index = getPosition(r,j);
                    elements.insert(elements.end(),data[index]);
                }
                return elements;
            }

            std::vector<float> column(int col){
                std::vector<float> elements;
                int i, index;
                for (i = 0; i < height; i++) {
                    index = getPosition(i,col);
                    elements.insert(elements.end(),data[index]);
                }
                return elements;
            }


            Matrix3 operator * (Matrix3 m){
                Matrix3 mul = Matrix3();
                int i,j,k,index;
                float currentValue = 0.0f;
                std::vector<float> r, c;
                std::vector<float>::size_type size_;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        r = row(i);
                        c = m.column(j);
                        size_ = c.size();
                        for (k = 0; k < (int)size_; k++) {
                            currentValue += r[k] * c[k];
                        }
                        index = getPosition(i,j);
                        mul.setElement(index,currentValue);
                        currentValue = 0.0;
                    }
                }
                return mul;
            }

            Vector3 operator * (Vector3 v){
                Vector3 mul = Vector3();
                mul.x = getElement(0,0) * v.x + getElement(0,1) * v.y + getElement(0,2) * v.z;
                mul.y = getElement(1,0) * v.x + getElement(1,1) * v.y + getElement(1,2) * v.z;
                mul.z = getElement(2,0) * v.x + getElement(2,1) * v.y + getElement(2,2) * v.z;
                return mul;    
            }

            bool operator == (const Matrix3 &m){
                float eps = std::numeric_limits<float>::epsilon();
                int i,j,index;
                float diff;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        diff = data[index] - m.getElement(i,j);
                        if(diff < eps && -diff < eps)
                            continue;
                        else
                            return false;
                    }
                }
                return true;
            }

            bool operator != (const Matrix3 &m){
                return !(*this == m);
            }

    };

    class Matrix4{
        private:
            int width  = 4;
            int height = 4;
            int getPosition(int i,int j) const {return (i + j * width);}

        public:
            std::array<float,16> data;
            Matrix4(){
                int i, j;
                for(i = 0; i < height; i++){
                    for(j = 0; j < width; j++){
                        data[i + j * width] = 0.0;
                    }
                }
            }

            Matrix4(const Matrix4 &m){
                int i, j, index;
                for(i = 0; i < height; i++){
                    for(j = 0; j < width; j++){
                        index = i + j * width;
                        data[index] = m.getElement(i,j);
                    }
                }
            }

            Matrix4(float values[16]){
                int i, j, index;
                for(i = 0; i < height; i++){
                    for(j = 0; j < width; j++){
                        index = i + j * width;
                        data[index] = values[j + i * height];
                    }
                }
            }

            Matrix4(std::vector<float> values){
                int i, j, index;
                for(i = 0; i < height; i++){
                    for(j = 0; j < width; j++){
                        index = i + j * width;
                        data[index] = values[j + i * height];
                    }
                }
            }

            Matrix4(float a00, float a01, float a02, float a03,
                    float a10, float a11, float a12, float a13,
                    float a20, float a21, float a22, float a23,
                    float a30, float a31, float a32, float a33){
                data[0]  = a00;
                data[1]  = a10;
                data[2]  = a20;
                data[3]  = a30;
                data[4]  = a01;
                data[5]  = a11;
                data[6]  = a21;
                data[7]  = a31;
                data[8]  = a02;
                data[9]  = a12;
                data[10] = a22;
                data[11] = a32;
                data[12] = a03;
                data[13] = a13;
                data[14] = a23;
                data[15] = a33;
            }
            // Matrix 4D Implementations

            float getElement(int i,int j) const {
                int index = getPosition(i, j);
                return data[index];
            }

            void setElement(int i,int j,float value){
                int index = getPosition(i, j);
                data[index] = value;
                return;
            }

            void setElement(int index, float value){
                data[index] = value;
                return;
            }

            Matrix4 operator = (Matrix4 m){
                int i,j,index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        data[index] = m.getElement(i,j);
                    }
                }
                return *this;
            }

            Matrix4 copy(){
                Matrix4 copypasta = Matrix4();
                int i,j,index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        copypasta.setElement(index,data[index]);
                    }
                }
                return copypasta;
            }

            Matrix4 translated(){
                Matrix4 trans = Matrix4();
                int i,j,index;
                for (j = 0; j < height; j++) {
                    for (i = 0; i < width; i++) {
                        index = getPosition(i,j);
                        trans.setElement(i,j,data[index]);
                    }
                }
                return trans;
            }

            void print(){
                clean();
                std::cout << "|";
                int i,j;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        if(j != width -1)
                            std::cout << std::setw(10) << getElement(i,j) << ", ";
                        else
                            std::cout << std::setw(10) << getElement(i,j) << "|" << std::endl;
                    }
                    if(i != height -1)
                        std::cout << "|";
                }
            }

            Matrix4 operator / (float k) {
                Matrix4 m = Matrix4();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] / k);
                    }
                }
                return m;
            }

            Matrix4 operator * (float k) {
                Matrix4 m = Matrix4();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] * k);
                    }
                }
                return m;
            }

            friend Matrix4 operator * (float k, const Matrix4 &m1) {
                int height = 4;
                int width =  4;
                Matrix4 m = Matrix4();
                int i, j;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        m.setElement(i,j,m1.getElement(i,j) * k);
                    }
                }
                return m;
            }

            Matrix4 operator + (float k) {
                Matrix4 m = Matrix4();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] + k);
                    }
                }
                return m;
            }

            Matrix4 operator - (float k) {
                Matrix4 m = Matrix4();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] - k);
                    }
                }
                return m;
            }

            Matrix4 operator + (Matrix4 m2) {
                Matrix4 m = Matrix4();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] + m2.getElement(i,j));
                    }
                }
                return m;
            }

            Matrix4 operator - (Matrix4 m2) {
                Matrix4 m = Matrix4();
                int i, j, index;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        m.setElement(index,data[index] - m2.getElement(i,j));
                    }
                }
                return m;
            }

            std::vector<float> row(int r){
                std::vector<float> elements;
                int index, j;
                for (j = 0; j < width; j++) {
                    index = getPosition(r,j);
                    elements.insert(elements.end(),data[index]);
                }
                return elements;
            }

            std::vector<float> column(int col){
                std::vector<float> elements;
                int i, index;
                for (i = 0; i < height; i++) {
                    index = getPosition(i,col);
                    elements.insert(elements.end(),data[index]);
                }
                return elements;
            }


            Matrix4 operator * (Matrix4 m){
                Matrix4 mul = Matrix4();
                int i,j,k,index;
                float currentValue = 0.0f;
                std::vector<float> r, c;
                std::vector<float>::size_type size_;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        r = row(i);
                        c = m.column(j);
                        size_ = c.size();
                        for (k = 0; k < (int)size_; k++) {
                            currentValue += r[k] * c[k];
                        }
                        index = getPosition(i,j);
                        mul.setElement(index,currentValue);
                        currentValue = 0.0;
                    }
                }
                return mul;
            }

            Vector4 operator * (Vector4 v){
                Vector4 mul = Vector4();
                mul.x = getElement(0,0) * v.x + getElement(0,1) * v.y + getElement(0,2) * v.z + getElement(0,3) * v.w;
                mul.y = getElement(1,0) * v.x + getElement(1,1) * v.y + getElement(1,2) * v.z + getElement(1,3) * v.w;
                mul.z = getElement(2,0) * v.x + getElement(2,1) * v.y + getElement(2,2) * v.z + getElement(2,3) * v.w;
                mul.w = getElement(3,0) * v.x + getElement(3,1) * v.y + getElement(3,2) * v.z + getElement(3,3) * v.w;
                return mul; 
            }

            bool operator == (const Matrix4 &m){
                float eps = std::numeric_limits<float>::epsilon();
                int i,j,index;
                float diff;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        diff = data[index] - m.getElement(i,j);
                        if(diff < eps && -diff < eps)
                            continue;
                        else
                            return false;
                    }
                }
                return true;
            }

            bool operator != (const Matrix4 &m){
                return !(*this == m);
            }

            float* getData(){
                return data.data();
            }

            void clean(){
                float eps = 1.0e-5f;
                int i,j,index;
                float diff;
                for (i = 0; i < height; i++) {
                    for (j = 0; j < width; j++) {
                        index = getPosition(i,j);
                        diff = data[index];
                        if(diff < eps && -diff < eps)
                            data[index] = 0.0;;
                    }
                }
            }

    };
};
#endif
