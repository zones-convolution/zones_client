#pragma once

#include <juce_opengl/juce_opengl.h>
#include <vector>

struct VertexBufferElement
{
    GLint count;
    GLuint type;
    GLboolean normalised;

    static GLuint GetSizeOfType (GLuint type)
    {
        switch (type)
        {
            case juce::gl::GL_FLOAT:
                return sizeof (GLfloat);
            case juce::gl::GL_UNSIGNED_INT:
                return sizeof (GLuint);
            case juce::gl::GL_UNSIGNED_BYTE:
                return sizeof (GLubyte);
            default:
                jassert (false);
                return 0;
        }
    }
};

class VertexBufferLayout
{
public:
    template <typename T>
    void Push (GLint count) = delete;

    template <>
    void Push<GLfloat> (GLint count)
    {
        elements_.push_back ({
            .count = count,
            .type = juce::gl::GL_FLOAT,
            .normalised = juce::gl::GL_FALSE,
        });
        stride_ += count * VertexBufferElement::GetSizeOfType (juce::gl::GL_FLOAT);
    }

    template <>
    void Push<GLuint> (GLint count)
    {
        elements_.push_back ({
            .count = count,
            .type = juce::gl::GL_UNSIGNED_INT,
            .normalised = juce::gl::GL_FALSE,
        });
        stride_ += count * VertexBufferElement::GetSizeOfType (juce::gl::GL_UNSIGNED_INT);
    }

    template <>
    void Push<GLubyte> (GLint count)
    {
        elements_.push_back ({
            .count = count,
            .type = juce::gl::GL_UNSIGNED_BYTE,
            .normalised = juce::gl::GL_TRUE,
        });
        stride_ += count * VertexBufferElement::GetSizeOfType (juce::gl::GL_UNSIGNED_BYTE);
    }

    [[nodiscard]] inline GLint GetStride () const
    {
        return stride_;
    }

    [[nodiscard]] inline const std::vector<VertexBufferElement> & GetElements () const
    {
        return elements_;
    }

private:
    std::vector<VertexBufferElement> elements_;
    GLint stride_ {};
};