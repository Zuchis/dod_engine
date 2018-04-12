#ifndef SCENE_H
#define SCENE_H

#include <unordered_map>

#include "mesh.h"
#include "shaderprogram.h"
#include "camera.h"
#include "object.h"

namespace engine {

    class RenderException : public std::runtime_error {
        public:
            RenderException( const std::string & msg ) :
                std::runtime_error(msg) { }
    };

    class SceneGraphException : public std::runtime_error {
        public:
            SceneGraphException( const std::string & msg ) :
                std::runtime_error(msg) { }
    };

    class SceneNode {
        private:
            SceneNode* parent;
            std::string tag;

            ShaderProgram* getProgramFromParent() {
                if(parent == nullptr)
                    return nullptr;

                if(parent->shaderProgram != nullptr)
                    return parent->shaderProgram;

                return parent->getProgramFromParent();
            }

        public:
            std::vector<SceneNode*> children;
            ShaderProgram* shaderProgram;
            Object* object;
            bool isRoot;

            SceneNode(std::string _tag = "") {
                shaderProgram = nullptr;
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

            void setShaderProgram(ShaderProgram* _program) {
                shaderProgram = _program;
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

            void draw()
                throw (RenderException) {
                    if (object == nullptr) {
                        throw RenderException("No Object set for the node " + tag);
                    }
                    if (shaderProgram == nullptr) {
                        shaderProgram = getProgramFromParent();
                        if (shaderProgram == nullptr) {
                            throw RenderException("Could not find any shader program");
                        }
                    }
                    shaderProgram->use();
                    shaderProgram->setUniform("Matrix",this->getModelMatrix().getData());
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
            ICamera* camera;

        public:
            SceneGraph() {
                root = new SceneNode("root");
                root->isRoot = true;
            }

            SceneNode* createNode(std::string tag = "") {
                return root->createNode(tag);
            }

            void setCamera(ICamera* _camera){
                camera = _camera;
            }

            ICamera* getCamera() {
                return camera;
            }

            SceneNode* getRoot() {
                return root;
            }

            void draw()
                throw (RenderException) {
                    if (!root->children.empty()){
                        int i;
                        for (i = 0; i < (int)root->children.size(); i++) {
                            root->children[i]->draw();

                        }
                    } else {
                        throw RenderException("The graph is empty.");
                    }
                }
    };

};

#endif

