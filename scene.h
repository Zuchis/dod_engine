#ifndef SCENE_H
#define SCENE_H

#include "shaderprogram.h"
#include "camera.h"
#include "object.h"

using std::vector;

namespace engine {

    struct NodeTags {
        std::string* tag;
    };

    struct NodeData {
        size_t* parents;
        Matrix4* locals;
        Matrix4* globals;
        ShaderData* shaders;
        MeshData* meshes;
    };

    //class SceneNode {
        //public:
            //void draw() {
                //glUseProgram(shaderData.programID);
                //glUniformMatrix4fv(shaderData.matrixID, 1, GL_FALSE, this->getModelMatrix().getData());
                //glBindVertexArray(mesh->vaoId);
                //glDrawArrays(GL_TRIANGLES,0,(GLsizei)mesh->vertices.size());
            //}
    //};
};
#endif
