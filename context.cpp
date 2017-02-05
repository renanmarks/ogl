#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <exception>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "context.h"

class ogl::Context::Impl
{
private:
    SDL_Window* window;
    SDL_GLContext context;

    void printOpenGLVersion() const
    {
        std::string glRender = reinterpret_cast<char const*>(glGetString(GL_RENDERER));
        std::string glVersion = reinterpret_cast<char const*>(glGetString(GL_VERSION));
        std::string glslVersion = reinterpret_cast<char const*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

        std::cout << "OpenGL Renderer  : " << glRender << '\n'
                  << "OpenGL Version   : " << glVersion << '\n'
                  << "OpenGLSL Version : " << glslVersion << std::endl;
    }

    void setupContext()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            throw std::runtime_error("Failed do init SDL video.");
        }

        this->window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);

        if (this->window == nullptr)
        {
            std::stringstream stream;
            stream << "Unable to create window" << std::endl;

            std::string error = SDL_GetError();

            if (error != "")
            {
                stream << "SDL Error : " << error << std::endl;
                SDL_ClearError();
            }

            throw std::runtime_error(stream.str());
        }

        this->setupContextAttributes();
        this->context = SDL_GL_CreateContext(this->window);

        if (GLEW_VERSION == 1 && GLEW_VERSION_MAJOR <= 13)
        {
            glewExperimental = GL_TRUE;
            //std::cout << "GLEW version: " << GLEW_VERSION << "." << GLEW_VERSION_MAJOR << "." << GLEW_VERSION_MINOR << "." <<GLEW_VERSION_MICRO << "\n";
            std::cout << "GLEW experimental enabled.\n";
        }

        GLenum error = glewInit();

        if (error != GLEW_OK)
        {
            throw std::runtime_error("Error initializing GLEW.");
        }

        // This makes our buffer swap syncronized with the monitor's vertical refresh
        SDL_GL_SetSwapInterval(1);

        printOpenGLVersion();
    }

    void setupContextAttributes() const
    {
        // Set our OpenGL version.
        // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        // Turn on double buffering with a 24bit Z buffer.
        // You may need to change this to 16 or 32 for your system
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // Clear our buffer with a black background
        // This is the same as :
        // 		SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
        // 		SDL_RenderClear(&renderer);
        //
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

public:
    Impl()
    {
        setupContext();
    }

    ~Impl()
    {
        // Delete our OpengL context
        SDL_GL_DeleteContext(this->context);

        // Destroy our window
        SDL_DestroyWindow(this->window);

        // Shutdown SDL 2
        SDL_Quit();
    }

    void swapWindow()
    {
        SDL_GL_SwapWindow(this->window);
    }
};

/*************************************************/

ogl::Context::Context()
    : impl(std::make_unique<ogl::Context::Impl>())
{

}

void ogl::Context::swapWindow()
{
    this->impl->swapWindow();
}

ogl::Context::~Context() = default;
