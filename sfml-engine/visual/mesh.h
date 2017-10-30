#ifndef MESH_H
#define MESH_H

#include "GL/glew.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include "../math/matrixfactory.h"

using math::Vector3;
using math::Vector2;

namespace engine {

    enum DataType{
        VERTICES = 0,
        TEXCOORDS = 1,
        NORMALS = 2
    };

    class Mesh {
        typedef std::array<float,3> vec3;
        typedef std::array<float,2> vec2;
        private:
            std::vector<vec3> vertices, normals;
            std::vector<vec2> texcoords;
            std::vector<Vector3> vertexData, normalData;
            std::vector<Vector2> texcoordData;
            bool texcoordsLoaded, normalsLoaded; 
            GLuint vaoId;
            GLuint vboVertices, vboTexcoords, vboNormals;
            std::vector<unsigned int> vertexIdx, texcoordIdx, normalIdx;

            void parseVertex(std::stringstream& sin) {
                Vector3 v = Vector3();
                sin >> v.x >> v.y >> v.z;
                vertexData.push_back(v);
            }

            void parseTexcoord(std::stringstream& sin) {
                Vector2 v = Vector2();
                sin >> v.x >> v.y;
                texcoordData.push_back(v);
            }

            void parseNormal(std::stringstream& sin) {
                Vector3 v = Vector3();
                sin >> v.x >> v.y >> v.z;
                normalData.push_back(v);
            }

            void parseFace(std::stringstream& sin) {
                std::string token;
                for(int i = 0; i < 3; i++) {
                    std::getline(sin, token, '/');
                    if(token.size() > 0) vertexIdx.push_back(std::stoi(token));
                    std::getline(sin, token, '/');
                    if(token.size() > 0) texcoordIdx.push_back(std::stoi(token));
                    std::getline(sin, token, ' ');
                    if(token.size() > 0) normalIdx.push_back(std::stoi(token));
                }
            }

            void parseLine(std::stringstream& sin){
                std::string s;
                sin >> s;
                if(s.compare("v") == 0)
                    parseVertex(sin);
                else if(s.compare("vt") == 0)
                    parseTexcoord(sin);
                else if(s.compare("vn") == 0)
                    parseNormal(sin);
                else if(s.compare("f" ) == 0)
                    parseFace(sin);
            }

            void loadMeshData(std::string& filename) {
                std::ifstream ifile(filename);
                while(ifile.good()) {
                    std::string line;
                    std::getline(ifile, line);
                    std::stringstream sin(line);
                    parseLine(sin);
                }
                texcoordsLoaded = (texcoordIdx.size() > 0);
                normalsLoaded = (normalIdx.size() > 0);
            }

            void processMeshData(){
                for(unsigned int i = 0; i < vertexIdx.size(); i++ ) {
                    unsigned int vi = vertexIdx[i];
                    Vector3 v = vertexData[vi - 1];
                    vertices.push_back(v.data);
                    if(texcoordsLoaded) {
                        unsigned int ti = texcoordIdx[i];
                        Vector2 t = texcoordData[ti - 1];
                        texcoords.push_back(t.data);
                    }
                    if(normalsLoaded) {
                        unsigned int ni = normalIdx[i];
                        Vector3 n = normalData[ni - 1];
                        normals.push_back(n.data);
                    }
                }
            }

            void freeMeshData() {
                vertexData.clear();
                texcoordData.clear();
                normalData.clear();
                vertexIdx.clear();
                texcoordIdx.clear();
                normalIdx.clear();
            }

        public:
            Mesh(std::string& filename) {
                loadMeshData(filename);
                processMeshData();
                freeMeshData();
            }

            void create() {
                glGenVertexArrays(1,&vaoId);
                glBindVertexArray(vaoId);
                {
                    glGenBuffers(1,&vboVertices);
                    glBindBuffer(GL_ARRAY_BUFFER,vboVertices);
                    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
                    glEnableVertexAttribArray(VERTICES);
                    glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), 0);

                }

                if(texcoordsLoaded) {
                    glGenBuffers(1, &vboTexcoords);
                    glBindBuffer(GL_ARRAY_BUFFER, vboTexcoords);
                    glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(texcoords[0]), &texcoords[0], GL_STATIC_DRAW);
                    glEnableVertexAttribArray(TEXCOORDS);
                    glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(texcoords[0]), 0);
                }

                if(normalsLoaded) {
                    glGenBuffers(1, &vboNormals);
                    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
                    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
                    glEnableVertexAttribArray(NORMALS);
                    glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(normals[0]), 0);
                }
                glBindVertexArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);

            }

            void draw() {
                glBindVertexArray(vaoId);
                glDrawArrays(GL_TRIANGLES,0,(GLsizei)vertices.size());
            }

            void print(){
                std::cout << "Vertices loaded: \n";
                int i, j;
                vec3 current;
                std::cout << vertices.size() << std::endl;
                for (i = 0; i < (int)(vertices.size()); i++) {
                    current = vertices[i];
                    for (j = 0; j < 3; j++) {
                        std::cout << current[j] << "\t";
                    }
                    std::cout << "\n";
                }
            }


            ~Mesh() {
                glBindVertexArray(vaoId);
                glDisableVertexAttribArray(VERTICES);
                glDisableVertexAttribArray(TEXCOORDS);
                glDisableVertexAttribArray(NORMALS);
                glDeleteBuffers(1, &vboVertices);
                glDeleteBuffers(1, &vboTexcoords);
                glDeleteBuffers(1, &vboNormals);
                glDeleteVertexArrays(1, &vaoId);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            }


    };

};

#endif

