#ifndef CONTEXT_H
#define CONTEXT_H
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

};

}

#endif // CONTEXT_H
