#include "BinaryData.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

class SpectrogramVisualiserComponent
    : public juce::Component
    , public juce::OpenGLRenderer
{
public:
    SpectrogramVisualiserComponent ();
    ~SpectrogramVisualiserComponent () override;

private:
    void newOpenGLContextCreated () override;
    void renderOpenGL () override;
    void openGLContextClosing () override;

    void resized () override;
    void paint (juce::Graphics & g) override;

    struct Vertex
    {
        float position [2];
        float colour [4];
    };

    std::vector<Vertex> vertex_buffer_;
    std::vector<unsigned int> index_buffer_;

    GLuint vertex_buffer_object_;
    GLuint index_buffer_object_;

    std::unique_ptr<juce::OpenGLShaderProgram> shader_program_;

    juce::OpenGLContext opengl_context_;
};