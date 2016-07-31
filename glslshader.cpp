#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include "GL/gl.h"

#include "glslshader.h"
#include <iostream>
#include <fstream>
#include <vector>

GLuint convertType(const ogl::GLSLShader::Type _type)
{
    switch (_type)
    {
    case ogl::GLSLShader::Type::FRAGMENT:
        return GL_FRAGMENT_SHADER;

    default:
    case ogl::GLSLShader::Type::VERTEX:
        return GL_VERTEX_SHADER;
    }
}

ogl::GLSLShader::GLSLShader(const std::string &_filename, const ogl::GLSLShader::Type _type)
    : filename(_filename), type(_type), glType(convertType(_type)), glId(glCreateShader(this->glType))
{
    this->loadAndCompileShader();
}

GLuint ogl::GLSLShader::getId() const
{
    return this->glId;
}

std::string ogl::GLSLShader::getFilename() const
{
    return this->filename;
}

void ogl::GLSLShader::loadAndCompileShader() const
{
    std::string shaderCode;

    // Read the Vertex Shader code from the file
    {
        std::ifstream shaderStream(this->filename, std::ios::in);

        if(shaderStream.good() == false)
        {
            throw std::runtime_error("Error when opening " + this->filename + ". Are you in the right directory ?");
        }

        shaderCode.assign( (std::istreambuf_iterator<char>(shaderStream) ),
                           (std::istreambuf_iterator<char>()             ) );

    } // End of scope, RAII shaderStream closed.

    char const * sourcePointer = shaderCode.c_str();
    GLint        result        = GL_FALSE;
    int          infoLogLength = 0;

    // Compile Shader
    std::cout << "Compiling shader : " << this->filename << std::endl;

    glShaderSource(this->getId(), 1, &sourcePointer , nullptr);
    glCompileShader(this->getId());

    // Check Shader
    glGetShaderiv(this->getId(), GL_COMPILE_STATUS, &result);
    glGetShaderiv(this->getId(), GL_INFO_LOG_LENGTH, &infoLogLength);

    if ( result == GL_FALSE )
    {
        std::vector<char> errorMessage(infoLogLength+1);
        glGetShaderInfoLog(this->getId(), infoLogLength, nullptr, &errorMessage[0]);

        std::cerr << "Error compiling " << this->getFilename() << '\n'
                  << "  " << std::string(errorMessage.begin(), errorMessage.end()) << std::endl;
    }
}

void ogl::GLSLShader::cleanup() const
{
    glDeleteShader(this->getId());
}
