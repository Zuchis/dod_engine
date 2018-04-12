#ifndef MANAGERS_H
#define MANAGERS_H

#include <unordered_map>
#include "shaderprogram.h"
#include "scene.h"
#include "mesh.h"

namespace engine {

    class MeshManager {
        private:
            typedef std::unordered_map<std::string,Mesh*> Map;

            Map map;

        protected:
            MeshManager() {}

        public:
            static MeshManager* instance() {
                static MeshManager manager;
                return &manager;
            }

            Mesh* get(std::string name)
            throw(RenderException) {
                Map::iterator it;
                it = map.find(name);
                //map[name]->print();
                if (it == map.end()){
                    throw(RenderException("No mesh called " + name + " found."));
                }

                return map[name];
            }

            void add(std::string name, Mesh* mesh){
                Map::iterator it;
                it = map.find(name);
                if (it != map.end()){
                    std::cout << "Mesh already exists, replacing it..." << std::endl;
                }
                map[name] = mesh;
            }
    };

    class ShaderProgramManager {
        private:
            typedef std::unordered_map<std::string,ShaderProgram*> Map;

            Map map;

        protected:
            ShaderProgramManager() {}

        public:
            static ShaderProgramManager* instance() {
                static ShaderProgramManager manager;
                return &manager;
            }

            ShaderProgram* get(std::string name)
            throw(ShaderProgramException) {
                Map::iterator it;
                it = map.find(name);
                if (it == map.end()){
                    throw(ShaderProgramException("No program called " + name + " found."));
                }

                return map[name];
            }

            void add(std::string name, ShaderProgram* program){
                Map::iterator it;
                it = map.find(name);
                if (it != map.end()){
                    std::cout << "ShaderProgram already exists, replacing it..." << std::endl;
                }
                map[name] = program;
            }
    };

    class SceneGraphManager {
        private:
            typedef std::unordered_map<std::string,SceneGraph*> Map;

            Map map;

        protected:
            SceneGraphManager() {}

        public:
            static SceneGraphManager* instance() {
                static SceneGraphManager manager;
                return &manager;
            }

            SceneGraph* get(std::string name)
            throw(SceneGraphException) {
                Map::iterator it;
                it = map.find(name);
                if (it == map.end()){
                    throw(SceneGraphException("No SceneGraph called " + name + " found."));
                }

                return map[name];
            }

            void add(std::string name, SceneGraph* program){
                Map::iterator it;
                it = map.find(name);
                if (it != map.end()){
                    std::cout << "SceneGraph already exists, replacing it..." << std::endl;
                }
                map[name] = program;
            }
    };

    class ObjectManager {
        private:
            typedef std::unordered_map<std::string,Object*> Map;

            Map objects;

        protected:
            ObjectManager() {}

        public:
            static ObjectManager* instance() {
                static ObjectManager manager;
                return &manager;
            }

            Object* get(std::string name)
            throw(RenderException) {
                Map::iterator it;
                it = objects.find(name);
                if (it == objects.end()){
                    throw(RenderException("No object called " + name + " found."));
                }

                return objects[name];
            }

            Map getObjects() {
                return objects;
            }

            void add(std::string name, Object* object){
                Map::iterator it;
                it = objects.find(name);
                if (it != objects.end()){
                    std::cout << "Object already exists, replacing it..." << std::endl;
                }
                objects[name] = object;
            }

            void updateObjects() {

                auto it = objects.begin();

                for(it = objects.begin(); it != objects.end(); it++) {
                    it->second->update();
                }
            }

            void calculateObjectsCollisionsWithBox(float xInf, float xSup, float yInf, float ySup, float zInf, float zSup) {
                auto it = objects.begin();

                for(it = objects.begin(); it != objects.end(); it++) {
                    it->second->calculateCollisionsWithBox(xInf, xSup, yInf, ySup, zInf, zSup);
                }
            }
    };

    class KeyBuffer {
        private:

            bool keys[256];
            bool specialKeys[256];

        protected:
            KeyBuffer() {
                std::fill_n(keys,256,false); 
                std::fill_n(specialKeys,256,false); 
            }

        public:
            static bool leftMouseButtonPressed;
            static bool rightMouseButtonPressed;
            static KeyBuffer* instance() {
                static KeyBuffer buffer;
                return &buffer;
            }

            void pressKey(unsigned char key) {
                keys[key] = true;
            }

            void pressSpecialKey(int key) {
                specialKeys[key] = true;
            }

            void releaseKey(unsigned char key) {
                keys[key] = false;
            }

            void releaseSpecialKey(int key) {
                specialKeys[key] = false;
            }

            bool isKeyDown(unsigned char key) {
                return keys[key];
            }

            bool isSpecialKeyDown(int key) {
                return specialKeys[key];
            }
    };
};

bool engine::KeyBuffer::leftMouseButtonPressed = false;
bool engine::KeyBuffer::rightMouseButtonPressed = false;

#endif
