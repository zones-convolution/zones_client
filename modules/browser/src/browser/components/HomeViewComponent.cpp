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

    import_zone_button_.onClick = [&]
    { tabs_context_.dispatch (LoadTabAction {.tab_name = "import"}); };
    addAndMakeVisible (import_zone_button_);

    load_from_disk_button_.onClick = [&] { LoadFromDisk (); };
    addAndMakeVisible (load_from_disk_button_);

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

void HomeViewComponent::LoadFromDisk ()
{
    directory_picker_ = std::make_unique<juce::FileChooser> ("Load IR from disk");
    auto directory_flags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    directory_picker_->launchAsync (
        directory_flags,
        [&] (const juce::FileChooser & chooser)
        {
            auto path = chooser.getResult ();
            if (path.exists ())
            {
                IrSelection ir_selection;
                auto & ir_metadata = ir_selection.ir;

                auto file_path = std::filesystem::path {path.getFullPathName ().toStdString ()};
                auto ir_filename = file_path.filename ();

                ir_metadata.title = ir_filename.stem ();
                ir_metadata.description = "Impulse response loaded from disk";
                ir_metadata.relative_path = file_path;
                ir_metadata.position_map = PositionMap {.centre = ""};

                ir_metadata.channel_format = GetChannelFormatFromFile (path);
                ir_selection.target_format = TargetFormat::kStereo;

                context_.dispatch (LoadIrAction {ir_selection});
            }
        });
}

ChannelFormat HomeViewComponent::GetChannelFormatFromFile (juce::File & path)
{
    juce::AudioFormatManager audio_format_manager;
    audio_format_manager.registerBasicFormats ();

    std::unique_ptr<juce::AudioFormatReader> reader (audio_format_manager.createReaderFor (path));

    auto num_channels = reader->numChannels;

    switch (num_channels)
    {
        case 2:
            return ChannelFormat::kStereo;
        case 4:
            return ChannelFormat::kFoa;

        default:
            return ChannelFormat::kMono;
    }
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
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::ButtonFlexItem (load_from_disk_button_));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void HomeViewComponent::Update (const HomeView & home_view)
{
}
