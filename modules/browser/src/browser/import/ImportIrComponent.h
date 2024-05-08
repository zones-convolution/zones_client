#pragma once

#include "format/IrData.h"
#include "look_and_feel/BoxIcons.h"
#include "look_and_feel/components/DividerComponent.h"
#include "look_and_feel/components/IconTextButton.h"

#include <juce_gui_basics/juce_gui_basics.h>

class SpeakerPositionComponent : public juce::Component
{
public:
    enum class SpeakerPosition
    {
        kC,
        kL,
        kR
    };

    enum class DisplayType
    {
        kC = 0,
        kLR = 1,
        kCLR = 2
    };

    SpeakerPositionComponent ();
    void resized () override;

    void UpdateVisiblePositions (DisplayType display_type);
    [[nodiscard]] const PositionMap & GetPositionMap () const;
    [[nodiscard]] int GetRequiredContentHeight () const;

private:
    static const juce::String kChoosePickerDialogTitle;

    void AddPath (SpeakerPosition speaker_position);
    void UpdateDisplay ();

    juce::Label centre_label_ {"Centre", "Centre"};
    juce::Label left_label_ {"Left", "Left"};
    juce::Label right_label_ {"Right", "Right"};

    juce::Label centre_path_label_ {"Centre Path"};
    juce::Label left_path_label_ {"Left Path"};
    juce::Label right_path_label_ {"Right Path"};

    IconTextButton centre_import_button_ {"Choose", BoxIcons::kBxFolder};
    IconTextButton left_import_button_ {"Choose", BoxIcons::kBxFolder};
    IconTextButton right_import_button_ {"Choose", BoxIcons::kBxFolder};

    PositionMap position_map_;

    std::unique_ptr<juce::FileChooser> file_picker_;
};

class ImportIrComponent : public juce::Component
{
public:
    ImportIrComponent ();
    void resized () override;

    [[nodiscard]] std::string GetTitle () const;
    [[nodiscard]] std::string GetDescription () const;
    [[nodiscard]] ChannelFormat GetChannelFormat () const;

    [[nodiscard]] int GetRequiredContentHeight () const;

    SpeakerPositionComponent speaker_position_component_;

    std::function<void ()> OnRemoveIr;

private:
    juce::Label import_ir_title_ {"Import IR", "Import IR"};
    IconTextButton remove_ir_button_ {"Remove IR", BoxIcons::kBxTrash};
    DividerComponent top_divider_;

    juce::TextEditor title_input_;
    juce::TextEditor description_input_;

    juce::ComboBox microphone_array_picker_;
    juce::ComboBox speaker_positions_picker_;

    juce::Label speaker_positions_title_ {"Speaker Positions", "Speaker Positions"};
    DividerComponent middle_divider_;

    static const std::map<std::string, ChannelFormat> kMicrophonePickerOptions;
};
