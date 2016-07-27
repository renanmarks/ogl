#include <iostream>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include "context.h"

int main(int argc, char *argv[])
{
    ogl::Context context;

    bool loop = true;

    while (loop)
    {
        SDL_Event event;

        if (SDL_WaitEvent(&event))
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
                    context.swapWindow();
                    break;
                case SDLK_g:
                    // Cover with green and update
                    glClearColor(0.0, 1.0, 0.0, 1.0);
                    glClear(GL_COLOR_BUFFER_BIT);
                    context.swapWindow();
                    break;
                case SDLK_b:
                    // Cover with blue and update
                    glClearColor(0.0, 0.0, 1.0, 1.0);
                    glClear(GL_COLOR_BUFFER_BIT);
                    context.swapWindow();
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

    return 0;
}
