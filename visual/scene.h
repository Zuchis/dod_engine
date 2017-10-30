#ifndef SCENE_H
#define SCENE_H

#include <unordered_map>

#include "mesh.h"
#include "shaderprogram.h"
#include "camera.h"
#include "texture.h"

namespace engine {

	class RenderException : public std::runtime_error {
	public:
		RenderException(const std::string & msg) :
			std::runtime_error(msg) { }
	};

	class SceneGraphException : public std::runtime_error {
	public:
		SceneGraphException(const std::string & msg) :
			std::runtime_error(msg) { }
	};

	class SceneNode {
	private:
		SceneNode* parent;
		std::string tag;
		bool drawable = true;

		ShaderProgram* getProgramFromParent() {
			if (parent == nullptr)
				return nullptr;

			if (parent->shaderProgram != nullptr)
				return parent->shaderProgram;

			return parent->getProgramFromParent();
		}

	public:
		std::vector<SceneNode*> children;
		math::Matrix4 modelMatrix;
		GLuint texture;
		Mesh* mesh;
		ShaderProgram* shaderProgram;
		bool isRoot;

		SceneNode(std::string _tag = "") {
			modelMatrix = math::Create4DIdentity();
			mesh = nullptr;
			shaderProgram = nullptr;
			parent = nullptr;
			isRoot = false;
			tag = _tag;
		}

		SceneNode* createNode(std::string tag = "") {
			SceneNode* child = new SceneNode(tag);
			child->setParent(this);
			children.push_back(child);
			return child;
		}

		void setMesh(Mesh* _mesh) {
			mesh = _mesh;
		}

		void setDrawable(bool b) {
			drawable = b;
		}

		void setTexture(std::string& filename)
		{
			texture = loadBMP_custom(filename.c_str());
		}

		void setShaderProgram(ShaderProgram* _program) {
			shaderProgram = _program;
		}

		void setParent(SceneNode* _parent) {
			parent = _parent;
		}

		math::Matrix4 getModelMatrix() {
			if (parent == nullptr || parent->isRoot)
				return modelMatrix;

			if (parent->parent->isRoot) {
				return parent->modelMatrix * modelMatrix;
			}

			return parent->getModelMatrix() * modelMatrix;
		}

		void setModelMatrix(math::Matrix4 matrix) {
			modelMatrix = matrix;
		}

		void applyMatrix(math::Matrix4 matrix) {
			modelMatrix = matrix * modelMatrix;
		}

		void setTag(std::string _tag) {
			tag = _tag;
		}

		std::string getTag() {
			return tag;
		}

		void draw()
			throw (RenderException) {
			if (mesh == nullptr) {
				throw RenderException("No Mesh set for the node " + tag);
			}
			if (shaderProgram == nullptr) {
				shaderProgram = getProgramFromParent();
				if (shaderProgram == nullptr) {
					throw RenderException("Could not find any shader program");
				}
			}
			shaderProgram->use();
			shaderProgram->setUniform("Matrix", this->getModelMatrix().getData());
			shaderProgram->setUniformTexture("Texture", texture);

			if (drawable)
				mesh->draw();

			if (!children.empty()) {
				int i;
				for (i = 0; i < (int)children.size(); i++) {
					children[i]->draw();
				}
			}
		}

		void applyMatrixToChildren(Matrix4 matrix) {
			if (!children.empty()) {
				int i;
				for (i = 0; i < (int)children.size(); i++) {
					children[i]->modelMatrix = matrix * children[i]->modelMatrix;
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

		void setCamera(ICamera* _camera) {
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
			if (!root->children.empty()) {
				int i;
				for (i = 0; i < (int)root->children.size(); i++) {
					root->children[i]->draw();

				}
			}
			else {
				throw RenderException("The graph is empty.");
			}
		}
	};

};

#endif

