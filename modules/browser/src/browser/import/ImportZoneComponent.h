#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

class ImportZoneComponent : public juce::Component
{
public:
    ImportZoneComponent ();

    [[nodiscard]] std::string GetTitle () const;
    [[nodiscard]] std::string GetDescription () const;
    [[nodiscard]] std::string GetUserPath () const;

    [[nodiscard]] int GetRequiredContentHeight () const;
    void resized () override;

private:
    juce::TextEditor title_input_;
    juce::TextEditor description_input_;
    juce::ComboBox user_path_picker_;
};