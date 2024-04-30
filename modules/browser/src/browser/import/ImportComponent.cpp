#include "ImportComponent.h"

#include "look_and_feel/LookAndFeel.h"

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

ImportComponent::ImportComponent ()
{
    addAndMakeVisible (import_title_);
    addAndMakeVisible (top_divider_);

    import_zone_button_.onClick = [&]
    {
        if (OnSubmit)
            OnSubmit ();
    };
    addAndMakeVisible (import_zone_);

    add_ir_button_.onClick = [&]
    {
        import_irs_.emplace_back (std::make_unique<ImportIrComponent> ());
        addAndMakeVisible (*import_irs_.back ());
        resized ();
    };
    addAndMakeVisible (add_ir_button_);
    addAndMakeVisible (import_zone_button_);
}

void ImportComponent::resized ()
{
    juce::FlexBox flex;
    LayoutHelper layout {flex};

    layout.UseColumn ();

    layout.AddLabel (import_title_);
    layout.AddHorizontalDivider (top_divider_);
    layout.AddFlex (import_zone_);

    for (auto & import_ir : import_irs_)
        layout.AddFlex (*import_ir);

    layout.AddButton (add_ir_button_);
    layout.AddButton (import_zone_button_, false);
    flex.performLayout (getLocalBounds ());
}