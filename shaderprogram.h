#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <iostream>
#include <unordered_map>
#include "matrixfactory.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

#define COLORS 1

namespace ShaderType {
    enum Type{
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER
    };
};

class ShaderProgramException : public std::runtime_error {
    public:
        ShaderProgramException( const std::string & msg ) :
            std::runtime_error(msg) { }
};

struct ShaderData {
    GLuint programID;
    GLint matrixID;
};

class ShaderProgram {
    private:
        GLuint programID;
        GLint matrixID;

    public:
        ShaderProgram(){
            programID = 0;
        }

        ~ShaderProgram(){
            GLint numShaders = 0;
            glGetProgramiv(programID, GL_ATTACHED_SHADERS, &numShaders);

            GLuint * shaderNames = new GLuint[numShaders];
            glGetAttachedShaders(programID, numShaders, NULL, shaderNames);

            // Deleting Shaders
            for (int i = 0; i < numShaders; i++)
                glDeleteShader(shaderNames[i]);

            // Deleting Program
            glDeleteProgram (programID);

            delete[] shaderNames;
        }

        void compileShaderFromFile(const char* fileName, ShaderType::Type type)
            throw( ShaderProgramException ){
                if(programID <= 0 ) {
                    programID = glCreateProgram();
                    if(programID == (GLuint)0) {
                        throw ShaderProgramException("Unable to create shader program.");
                    }
                }

                std::ifstream inFile( fileName, std::ios::in );
                if( !inFile ) {
                    std::string message = std::string("Unable to open: ") + fileName;
                    throw ShaderProgramException(message);
                }

                std::stringstream code;
                code << inFile.rdbuf();
                inFile.close();

                std::string source = code.str();
                GLuint shaderID = glCreateShader(type);

                const char * c_code = source.c_str();
                glShaderSource(shaderID, 1, &c_code, NULL );

                glCompileShader(shaderID);

                int result;

                glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result );
                if( GL_FALSE == result ) {
                    int length = 0;
                    std::string logString;
                    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length );
                    if( length > 0 ) {
                        char * c_log = new char[length];
                        int written = 0;
                        glGetShaderInfoLog(shaderID, length, &written, c_log);
                        logString = c_log;
                        delete [] c_log;
                    }
                    std::string msg;
                    if( fileName ) {
                        msg = std::string(fileName) + ": shader compliation failed\n";
                    } else {
                        msg = "Shader compilation failed.\n";
                    }
                    msg += logString;

                    throw ShaderProgramException(msg);

                } else {
                    glAttachShader(programID, shaderID);
                }
            }

        void bindAttribLocation(GLuint location, const char* name){
            glBindAttribLocation(programID,location,name);
        }

        void bindFragDataLocation(GLuint location, const char* name){
            glBindFragDataLocation(programID, location,name);
        }

        int getProgramID(){
            return programID;
        }

        void link() throw(ShaderProgramException){
            if( programID <= 0 )
                throw ShaderProgramException("Program has not been compiled.");

            glLinkProgram(programID);

            int status = 0;
            glGetProgramiv( programID, GL_LINK_STATUS, &status);
            if(status == GL_FALSE) {
                int length = 0;
                std::string logString;

                glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length );

                if( length > 0 ) {
                    char * c_log = new char[length];
                    int written = 0;
                    glGetProgramInfoLog(programID, length, &written, c_log);
                    logString = c_log;
                    delete [] c_log;
                }

                throw ShaderProgramException(std::string("Program link failed:\n") + logString);
            }
        }

        void use() {
            glUseProgram(programID);
        }

        void setMatrixID() { 
            this->matrixID = glGetUniformLocation(programID, "Matrix");
        }

        void setModelMatrix(float* m) {
            glUniformMatrix4fv(matrixID, 1, GL_FALSE, m);
        }

        void setUniform(GLint uniformID, float* m) { 
            glUniformMatrix4fv(uniformID,1,GL_FALSE,m);
        }

        void addUniformBlock(const char* name, GLuint location) {
            GLint uboId = glGetUniformBlockIndex(this->programID,name);
            glUniformBlockBinding(this->programID, uboId, location);
        }
};

void compileShaderFromFile(GLuint programID, const char* fileName, ShaderType::Type type) {
    std::ifstream inFile( fileName, std::ios::in );
    if( !inFile ) {
        std::cout << std::string("Unable to open: ") + fileName << std::endl;
        exit(EXIT_FAILURE);
    }

    std::stringstream code;
    code << inFile.rdbuf();
    inFile.close();

    std::string source = code.str();
    GLuint shaderID = glCreateShader(type);

    const char * c_code = source.c_str();
    glShaderSource(shaderID, 1, &c_code, NULL );

    glCompileShader(shaderID);

    int result;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result );
    if( GL_FALSE == result ) {
        int length = 0;
        std::string logString;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length );
        if( length > 0 ) {
            char * c_log = new char[length];
            int written = 0;
            glGetShaderInfoLog(shaderID, length, &written, c_log);
            logString = c_log;
            delete [] c_log;
        }
        std::string msg;
        if( fileName ) {
            msg = std::string(fileName) + ": shader compliation failed\n";
        } else {
            msg = "Shader compilation failed.\n";
        }
        msg += logString;

        std::cout << msg << std::endl;

        exit(EXIT_FAILURE);
    } else {
        glAttachShader(programID, shaderID);
    }
}

void bindAttribLocation(GLuint programID, GLuint location, const char* name){
    glBindAttribLocation(programID,location,name);
}

void addUniformBlock(GLuint programID, const char* name, GLuint location) {
    GLint uboId = glGetUniformBlockIndex(programID,name);
    glUniformBlockBinding(programID, uboId, location);
}

void link(GLuint programID) {
    glLinkProgram(programID);

    int status = 0;
    glGetProgramiv(programID, GL_LINK_STATUS, &status);
    if(status == GL_FALSE) {
        int length = 0;
        std::string logString;

        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length );

        if( length > 0 ) {
            char * c_log = new char[length];
            int written = 0;
            glGetProgramInfoLog(programID, length, &written, c_log);
            logString = c_log;
            delete [] c_log;
        }

        std::cout << std::string("Program link failed:\n") + logString << std::endl;
        exit(EXIT_FAILURE);
    }
}

GLint getMatrixID(GLuint programID) { 
    return glGetUniformLocation(programID, "Matrix");
}

#endif
