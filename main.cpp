#include <iostream>
#include <SDL2/SDL.h>

#define GL3_PROTOTYPES 1
#include <GL/gl.h>

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed do init SDL video." << std::endl;
        return 1;
    }

    SDL_Window* mainWindow = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL);

    if (mainWindow == NULL)
    {
        std::cout << "Unable to create window" << std::endl;

        int line = __LINE__;

        std::string error = SDL_GetError();

        if (error != "")
        {
            std::cout << "SLD Error : " << error << std::endl;

            if (line != -1)
                std::cout << "\nLine : " << line << std::endl;

            SDL_ClearError();
        }
    }

    SDL_GLContext mainContext = SDL_GL_CreateContext(mainWindow);

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
    SDL_GL_SwapWindow(mainWindow);

    bool loop = true;

    while (loop)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                loop = false;

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    loop = false;
                    break;
                case SDLK_r:
                    // Cover with red and update
                    glClearColor(1.0, 0.0, 0.0, 1.0);
                    glClear(GL_COLOR_BUFFER_BIT);
                    SDL_GL_SwapWindow(mainWindow);
                    break;
                case SDLK_g:
                    // Cover with green and update
                    glClearColor(0.0, 1.0, 0.0, 1.0);
                    glClear(GL_COLOR_BUFFER_BIT);
                    SDL_GL_SwapWindow(mainWindow);
                    break;
                case SDLK_b:
                    // Cover with blue and update
                    glClearColor(0.0, 0.0, 1.0, 1.0);
                    glClear(GL_COLOR_BUFFER_BIT);
                    SDL_GL_SwapWindow(mainWindow);
                    break;
                default:
                    break;
                }
            }
        }

        // Swap our back buffer to the front
        // This is the same as :
        // 		SDL_RenderPresent(&renderer);
    }

    // Delete our OpengL context
    SDL_GL_DeleteContext(mainContext);

    // Destroy our window
    SDL_DestroyWindow(mainWindow);

    // Shutdown SDL 2
    SDL_Quit();

    return 0;
}
