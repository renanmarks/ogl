#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <iostream>
#include "glslprogram.h"

ogl::GLSLProgram::GLSLProgram()
    : glId(glCreateProgram())
{

}

void ogl::GLSLProgram::addShader(const ogl::GLSLShader& shader)
{
    this->shaders.push_back(shader);
}

void ogl::GLSLProgram::linkProgram()
{
    std::cout << "Linking shader programs:" << std::endl;

    for (const ogl::GLSLShader& shader : this->shaders)
    {
        std::cout << "  " << shader.getFilename() << "..." << std::endl;
        glAttachShader(this->getId(), shader.getId());
    }

    // Setup Vertex Attributes (only for GL < 3.3 and GLSL < 3.3)
    // glBindAttribLocation (this->getId(), 0, "vertexPosition_modelspace");

    glLinkProgram(this->getId());

    GLint result        = GL_FALSE;
    int   InfoLogLength = 0;

    // Check the program
    glGetProgramiv(this->getId(), GL_LINK_STATUS, &result);
    glGetProgramiv(this->getId(), GL_INFO_LOG_LENGTH, &InfoLogLength);

    if ( result == GL_FALSE )
    {
        std::vector<char> errorMessage(InfoLogLength+1);
        glGetProgramInfoLog(this->getId(), InfoLogLength, nullptr, &errorMessage[0]);

        std::cerr << std::string(errorMessage.begin(), errorMessage.end()) << std::endl;
    }

    for (const ogl::GLSLShader& shader : this->shaders)
    {
        glDetachShader(this->getId(), shader.getId());
        shader.cleanup();
    }

    std::cout << "done." << std::endl;
}

GLuint ogl::GLSLProgram::getId() const
{
    return this->glId;
}
