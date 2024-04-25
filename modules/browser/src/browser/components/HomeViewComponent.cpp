#include "HomeViewComponent.h"

#include "Top10ViewComponent.h"
#include "browser/components/cards/UserZoneCard.h"
#include "look_and_feel/LookAndFeel.h"

const juce::String HomeViewComponent::kPathPickerDialogTitle = "Pick Project Directory";

HomeViewComponent::HomeViewComponent (lager::store<BrowserAction, BrowserModel> & browser_store,
                                      const lager::reader<Model> & model,
                                      lager::context<Action> & context)
    : context_ (context)
    , browser_context_ (browser_store)
    , ir_reader_ (model [&Model::ir_repository_model][&IrRepositoryModel::current_ir_metadata])
    , ir_repository_reader_ (model [&Model::ir_repository_model])
    , user_irs_reader_ (model [&Model::ir_repository_model][&IrRepositoryModel::user_irs])
{
    addAndMakeVisible (add_path_button_);
    addAndMakeVisible (card_banner_grid_);
    addAndMakeVisible (top_divider_);
    addAndMakeVisible (top_label_);

    lager::watch (user_irs_reader_, [&] (const auto &) { UpdateIrList (); });
    add_path_button_.onClick = [&] { AddPath (); };
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
        {
            auto zone_user_card = std::make_unique<UserZoneCard> (user_ir, ir_reader_);
            zone_user_card->OnLoad = [&, display_name]
            {
                context_.dispatch (
                    LoadIrAction {.ir_metadata = user_ir, .target_format = TargetFormat::kStereo});
            };

            zone_user_card->OnView = [&] { browser_context_.dispatch (LoadZoneAction {}); };

            cards_.push_back (std::move (zone_user_card));
        }
    }

    card_banner_grid_.Update ();
}

void HomeViewComponent::AddPath ()
{
    directory_picker_ = std::make_unique<juce::FileChooser> (kPathPickerDialogTitle);
    auto directory_flags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;

    directory_picker_->launchAsync (directory_flags,
                                    [&] (const juce::FileChooser & chooser)
                                    {
                                        auto path_name =
                                            chooser.getResult ().getFullPathName ().toStdString ();
                                        current_paths_ = current_paths_.push_back (
                                            chooser.getResult ().getFullPathName ().toStdString ());
                                        context_.dispatch (RefreshUserIrsAction {});
                                    });
}

void HomeViewComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (LookAndFeel::ButtonFlexItem (add_path_button_));
    layout.items.add (LookAndFeel::kDoubleFlexSpacer);
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
