#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/gl.h>

#include "context.h"
#include <SDL2/SDL.h>

#include <exception>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

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

        setupContextAttributes();

        this->window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL);

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

        this->context = SDL_GL_CreateContext(this->window);

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

    void compileShader(const std::string &filePath, GLuint shaderID) const
    {
        std::string shaderCode;

        // Read the Vertex Shader code from the file
        {
            std::ifstream shaderStream(filePath, std::ios::in);

            if(shaderStream.good() == false)
            {
                throw std::runtime_error("Error when opening " + filePath + ". Are you in the right directory ?");
            }

            shaderCode.assign( (std::istreambuf_iterator<char>(shaderStream) ),
                               (std::istreambuf_iterator<char>()             ) );

        } // End of scope, RAII shaderStream closed.

        char const * sourcePointer = shaderCode.c_str();
        GLint        result        = GL_FALSE;
        int          infoLogLength = 0;

        // Compile Shader
        std::cout << "Compiling shader : " << filePath << std::endl;

        glShaderSource(shaderID, 1, &sourcePointer , nullptr);
        glCompileShader(shaderID);

        // Check Shader
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

        if ( result == GL_FALSE )
        {
            std::vector<char> errorMessage(infoLogLength+1);
            glGetShaderInfoLog(shaderID, infoLogLength, nullptr, &errorMessage[0]);

            std::cout << infoLogLength << std::string(errorMessage.begin(), errorMessage.end()) << std::endl;
        }
    }

    GLuint linkProgram(GLuint VertexShaderID, GLuint FragmentShaderID) const
    {
        GLuint programID = glCreateProgram();

        std::cout << "Linking shader programs... ";

        glAttachShader(programID, VertexShaderID);
        glAttachShader(programID, FragmentShaderID);

        // Setup Vertex Attributes (only for GL < 3.3 and GLSL < 3.3)
        // glBindAttribLocation (ProgramID, 0, "vertexPosition_modelspace");

        glLinkProgram(programID);

        GLint result        = GL_FALSE;
        int   InfoLogLength = 0;

        // Check the program
        glGetProgramiv(programID, GL_LINK_STATUS, &result);
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);

        if ( result == GL_FALSE )
        {
            std::vector<char> errorMessage(InfoLogLength+1);
            glGetProgramInfoLog(programID, InfoLogLength, nullptr, &errorMessage[0]);

            std::cout << std::string(errorMessage.begin(), errorMessage.end()) << std::endl;
        }

        glDetachShader(programID, VertexShaderID);
        glDetachShader(programID, FragmentShaderID);

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

        std::cout << "done." << std::endl;

        return programID;
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

    GLuint getShadersProgramID(const std::string &vertexFilePath, const std::string &fragmentFilePath) const
    {
        // Create the shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        // Compile Vertex Shader
        compileShader(vertexFilePath, VertexShaderID);

        // Compile Fragment Shader
        compileShader(fragmentFilePath, FragmentShaderID);

        // Link the program
        GLuint ProgramID = linkProgram(VertexShaderID, FragmentShaderID);

        return ProgramID;
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

GLint ogl::Context::getShadersProgramID(const std::string &vertexFilePath, const std::string &fragmentFilePath) const
{
    return this->impl->getShadersProgramID(vertexFilePath, fragmentFilePath);
}

ogl::Context::~Context() = default;
