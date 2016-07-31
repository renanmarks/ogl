#ifndef GLSLSHADER_H
#define GLSLSHADER_H

#include <string>
#include <GL/gl.h>

namespace ogl
{
class GLSLShader
{
public:
    enum Type
    {
        VERTEX, FRAGMENT
    };

    GLSLShader();
    GLSLShader(const std::string& _filename, const Type _type);
    GLuint getId() const;
    std::string getFilename() const;
    void loadAndCompileShader() const;
    void cleanup() const;

private:
    const std::string filename;
    const ogl::GLSLShader::Type type;
    GLuint glType;
    GLuint glId;

};
}

#endif // GLSLSHADER_H
