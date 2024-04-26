#include "HomeViewComponent.h"

#include "browser/components/cards/UserZoneCard.h"
#include "look_and_feel/LookAndFeel.h"

HomeViewComponent::HomeViewComponent (lager::store<BrowserAction, BrowserModel> & browser_store,
                                      const lager::reader<Model> & model,
                                      lager::context<Action> & context)
    : model_ (model)
    , context_ (context)
    , browser_context_ (browser_store)
    , ir_repository_reader_ (model [&Model::ir_repository_model])
    , ir_reader_ (model [&Model::ir_repository_model][&IrRepositoryModel::current_ir_metadata])
    , user_irs_reader_ (model [&Model::ir_repository_model][&IrRepositoryModel::user_irs])
{
    addAndMakeVisible (card_banner_grid_);
    addAndMakeVisible (top_divider_);
    addAndMakeVisible (top_label_);

    lager::watch (user_irs_reader_, [&] (const auto &) { UpdateIrList (); });
    UpdateIrList ();
}

void HomeViewComponent::UpdateIrList ()
{
    auto user_irs = user_irs_reader_.get ();
    cards_.clear ();

    for (auto & user_ir : user_irs)
    {
        auto display_name = user_ir.name;
        auto channel_format = user_ir.channel_format;
        if (channel_format.has_value () && display_name.has_value ())
            cards_.push_back (
                std::make_unique<UserZoneCard> (user_ir, model_, context_, browser_context_));
    }

    card_banner_grid_.Update ();
}

void HomeViewComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (LookAndFeel::LabelFlexItem (top_label_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::HorizontalDividerFlexItem (top_divider_));
    layout.items.add (LookAndFeel::kDoubleFlexSpacer);
    layout.items.add (juce::FlexItem (card_banner_grid_).withHeight (180.0f));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void HomeViewComponent::Update (const HomeView & home_view)
{
}
