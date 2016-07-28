#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

// Only for GL < 3.3 and GLSL < 3.3
//in vec3 vertexPosition_modelspace;

void main()
{
    gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w = 1.0;
}
