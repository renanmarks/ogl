#include "context.h"

#define GL3_PROTOTYPES 1
#include <GL/gl.h>
#include <SDL2/SDL.h>

#include <exception>
#include <sstream>

class ogl::Context::Impl
{
private:
    SDL_Window* window;
    SDL_GLContext context;

    void setupContext()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            throw std::runtime_error("Failed do init SDL video.");
        }

        this->window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL);

        if (this->window == NULL)
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

        this->context = SDL_GL_CreateContext(this->window);
    }

    void setupContextAttributes()
    {
        // Set our OpenGL version.
        // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

        // Turn on double buffering with a 24bit Z buffer.
        // You may need to change this to 16 or 32 for your system
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // This makes our buffer swap syncronized with the monitor's vertical refresh
        SDL_GL_SetSwapInterval(1);

        // Clear our buffer with a black background
        // This is the same as :
        // 		SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
        // 		SDL_RenderClear(&renderer);
        //
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(this->window);
    }

public:
    Impl()
    {
        setupContext();
        setupContextAttributes();
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
