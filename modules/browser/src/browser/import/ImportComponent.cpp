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
    addAndMakeVisible (import_zone_button_);

    SetupContentView ();

    addAndMakeVisible (viewport_);
    viewport_.setViewedComponent (&content_, false);
}

void ImportComponent::UpdateImportIrs ()
{
    for (auto i = 0; i < import_irs_.size (); ++i)
        import_irs_ [i]->OnRemoveIr = [&, i]
        {
            import_irs_.erase (import_irs_.begin () + i);
            UpdateImportIrs ();
        };
    resized ();
}

void ImportComponent::SetupContentView ()
{
    content_.addAndMakeVisible (import_zone_);

    add_ir_button_.onClick = [&]
    {
        import_irs_.emplace_back (std::make_unique<ImportIrComponent> ());
        content_.addAndMakeVisible (*import_irs_.back ());
        UpdateImportIrs ();
    };

    content_.addAndMakeVisible (add_ir_button_);
}

int ImportComponent::GetRequiredContentHeight () const
{
    int import_ir_height = 0;
    for (auto & import_ir : import_irs_)
        import_ir_height += import_ir->GetRequiredContentHeight ();

    return import_zone_.GetRequiredContentHeight () +
           static_cast<int> (LookAndFeel::kButtonHeight) + import_ir_height +
           (2 * static_cast<int> (LookAndFeel::kGap));
}

void ImportComponent::LayoutContent ()
{
    auto viewport_visible_width = viewport_.getMaximumVisibleWidth ();
    auto content_bounds = getLocalBounds ()
                              .withWidth (viewport_visible_width)
                              .withHeight (GetRequiredContentHeight ());
    if (viewport_.canScrollVertically ())
        content_bounds.removeFromRight (LookAndFeel::kPadding);

    content_.setBounds (content_bounds);

    juce::FlexBox flex;
    LayoutHelper layout {flex};

    layout.UseColumn ();

    layout.AddFlex (import_zone_);
    for (auto & import_ir : import_irs_)
        flex.items.add (
            juce::FlexItem (*import_ir).withHeight (import_ir->GetRequiredContentHeight ()));
    layout.AddButton (add_ir_button_);

    flex.performLayout (content_bounds);
}

void ImportComponent::resized ()
{
    juce::FlexBox flex;
    LayoutHelper layout {flex};

    layout.UseColumn ();

    layout.AddLabel (import_title_);
    layout.AddHorizontalDivider (top_divider_);
    layout.AddFlex (viewport_);
    layout.AddButton (import_zone_button_, false);

    flex.performLayout (getLocalBounds ());

    auto view_position = viewport_.getViewPosition ();
    auto viewport_visible_width = viewport_.getMaximumVisibleWidth ();
    LayoutContent ();
    if (viewport_.getViewWidth () != viewport_visible_width)
        LayoutContent ();
    viewport_.setViewPosition (view_position);
}
