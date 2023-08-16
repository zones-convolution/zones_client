#pragma once
#include <juce_opengl/juce_opengl.h>

static void GLClearError ()
{
    while (juce::gl::glGetError () != juce::gl::GL_NO_ERROR)
        ;
}

static bool GLLogCall (const char * function, const char * file, int line)
{
    while (auto error = juce::gl::glGetError ())
    {
        DBG ("[OpenGL Error] (" + std::to_string (error) + ") " + function + " " + file + " " +
             std::to_string (line));
        return false;
    }
    return true;
}

/*
 * WILL NOT WORK WITH SINGLE LINE IF!!
 */
#define GLCall(x)    \
    GLClearError (); \
    x;               \
    jassert (GLLogCall (#x, __FILE__, __LINE__))