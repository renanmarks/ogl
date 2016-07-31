#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <GL/gl.h>
#include <vector>
#include "glslshader.h"

namespace ogl
{
class GLSLProgram
{
private:
    std::vector<ogl::GLSLShader> shaders;
    GLuint glId;

public:
    GLSLProgram();

    void addShader(const ogl::GLSLShader& shader);
    void linkProgram();

    GLuint getId() const;
};
}

#endif // GLSLPROGRAM_H
