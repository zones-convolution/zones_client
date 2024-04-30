#include "HomeViewComponent.h"

#include "browser/components/cards/UserZoneCard.h"
#include "look_and_feel/LookAndFeel.h"

HomeViewComponent::HomeViewComponent (lager::store<BrowserAction, BrowserModel> & browser_store,
                                      const lager::reader<Model> & model,
                                      lager::context<Action> & context,
                                      lager::context<TabsAction> & tabs_context)
    : model_ (model)
    , context_ (context)
    , browser_context_ (browser_store)
    , tabs_context_ (tabs_context)
    , zones_repository_reader_ (model [&Model::zone_repository_model])
    , ir_reader_ (model [&Model::zone_repository_model][&ZoneRepositoryModel::current_ir])
    , user_zones_reader_ (model [&Model::zone_repository_model][&ZoneRepositoryModel::user_zones])
{
    addAndMakeVisible (card_banner_grid_);
    addAndMakeVisible (top_divider_);
    addAndMakeVisible (top_label_);
    addAndMakeVisible (import_zone_button_);

    import_zone_button_.onClick = [&]
    { tabs_context_.dispatch (LoadTabAction {.tab_name = "import"}); };

    lager::watch (user_zones_reader_, [&] (const auto &) { UpdateZoneList (); });
    UpdateZoneList ();
}

void HomeViewComponent::UpdateZoneList ()
{
    cards_.clear ();

    auto user_zones = user_zones_reader_.get ();
    for (auto & user_zone : user_zones)
        cards_.push_back (
            std::make_unique<UserZoneCard> (user_zone, model_, context_, browser_context_));

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
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::ButtonFlexItem (import_zone_button_));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void HomeViewComponent::Update (const HomeView & home_view)
{
}
