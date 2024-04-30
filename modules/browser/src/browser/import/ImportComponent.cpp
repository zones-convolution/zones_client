#include "ImportComponent.h"

#include "look_and_feel/LookAndFeel.h"

ImportComponent::ImportComponent ()
{
    addAndMakeVisible (import_title_);
    addAndMakeVisible (top_divider_);
    addAndMakeVisible (import_zone_);

    import_ir_button_.onClick = [&]
    {
        if (OnSubmit)
            OnSubmit ();
    };
    addAndMakeVisible (import_ir_button_);
}

class LayoutHelper
{
public:
    explicit LayoutHelper (juce::FlexBox & layout)
        : layout_ (layout)
    {
    }

    void UseColumn ()
    {
        layout_.flexDirection = juce::FlexBox::Direction::column;
    }

    void AddSpacer ()
    {
        layout_.items.add (LookAndFeel::kFlexSpacer);
    }

    void AddLabel (juce::Label & label, bool with_spacer = true)
    {
        layout_.items.add (LookAndFeel::LabelFlexItem (label));
        if (with_spacer)
            AddSpacer ();
    }

    void AddHorizontalDivider (DividerComponent & divider, bool with_spacer = true)
    {
        layout_.items.add (LookAndFeel::HorizontalDividerFlexItem (divider));
        if (with_spacer)
            AddSpacer ();
    }

    void AddButton (juce::Component & component, bool with_spacer = true)
    {
        layout_.items.add (LookAndFeel::ButtonFlexItem (component));
        if (with_spacer)
            AddSpacer ();
    }

    void AddFlex (juce::Component & component, bool with_spacer = true)
    {
        layout_.items.add (juce::FlexItem (component).withFlex (1.0f));
        if (with_spacer)
            AddSpacer ();
    }

private:
    juce::FlexBox & layout_;
};

void ImportComponent::resized ()
{
    juce::FlexBox flex;
    LayoutHelper layout {flex};

    layout.UseColumn ();

    layout.AddLabel (import_title_);
    layout.AddHorizontalDivider (top_divider_);
    layout.AddFlex (import_zone_, false);

    layout.AddButton (import_ir_button_, false);

    flex.performLayout (getLocalBounds ());
}