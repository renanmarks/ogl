#ifndef CONTEXT_H
#define CONTEXT_H

#include <GL/gl.h>
#include <string>
#include <memory>

namespace ogl
{

class Context
{
private:
    class Impl;
    std::unique_ptr<Impl> impl;

public:
    Context();
    ~Context();

    void swapWindow();
    GLint getShadersProgramID(const std::string& vertexFilePath, const std::string& fragmentFilePath) const;

};

}

#endif // CONTEXT_H
