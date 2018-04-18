#ifndef SCENE_H
#define SCENE_H

#include <unordered_map>

#include "mesh.h"
#include "shaderprogram.h"
#include "camera.h"
#include "object.h"

namespace engine {

    class SceneNode {
        private:
            SceneNode* parent;
            std::string tag;

        public:
            std::vector<SceneNode*> children;
            ShaderData shaderData;
            Object* object;
            bool isRoot;

            SceneNode(std::string _tag = "") {
                parent = nullptr;
                isRoot = false;
                tag = _tag;
                object = nullptr;
            }

            SceneNode* createNode(std::string tag = "") {
                SceneNode* child = new SceneNode(tag);
                child->setParent(this);
                children.push_back(child);
                return child;
            }

            void setObject(Object* _object) {
                object = _object;
            }

            void setMesh(Mesh* _mesh) {
                object->setMesh(_mesh);
            }


            void setParent(SceneNode* _parent) {
                parent = _parent;
            }

            math::Matrix4 getModelMatrix() {
                if(parent == nullptr || parent->isRoot)
                    return object->modelMatrix();

                    if(parent->parent->isRoot){
                        return parent->object->modelMatrix() * object->modelMatrix();
                    }
                    
                    return parent->getModelMatrix() * object->modelMatrix();
            }

            void translateNode(Vector3 t) {
                object->translateObject(t);
            }

            void rotateNode(Quaternion r) {
                object->rotateObject(r);
            }

            void scaleNode(Vector3 s) {
                object->scaleObject(s);
            }

            void setTag(std::string _tag) {
                tag = _tag;
            }

            std::string getTag() {
                return tag;
            }

            void draw() {
                glUseProgram(shaderData.programID);
                glUniformMatrix4fv(shaderData.matrixID, 1, GL_FALSE, this->getModelMatrix().getData());

                object->draw();
                if (!children.empty()){
                    int i;
                    for (i = 0; i < (int)children.size(); i++) {
                        children[i]->draw();
                    }
                }
            }
    };

    class SceneGraph {
        private:
            SceneNode* root;
            ArcballCamera* camera;
            GLint matrixID;

        public:
            SceneGraph() {
                root = new SceneNode("root");
                root->isRoot = true;
            }

            SceneNode* createNode(std::string tag = "") {
                return root->createNode(tag);
            }

            void setCamera(ArcballCamera* _camera){
                camera = _camera;
            }

            ArcballCamera* getCamera() {
                return camera;
            }

            SceneNode* getRoot() {
                return root;
            }

            void draw() {
                if (!root->children.empty()){
                    int i;
                    for (i = 0; i < (int)root->children.size(); i++) {
                        root->children[i]->draw();

                    }
                }
            }
    };

};

#endif

