#include "LoadFromDiskController.h"

LoadFromDiskController::LoadFromDiskController (lager::context<Action> & context,
                                                const lager::reader<Model> & model)
    : context_ (context)
    , valid_target_formats_reader_ (
          model [&Model::zone_repository_model][&ZoneRepositoryModel::valid_target_formats])
{
}

void LoadFromDiskController::Load ()
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

                juce::AudioFormatManager audio_format_manager;
                audio_format_manager.registerBasicFormats ();

                std::unique_ptr<juce::AudioFormatReader> reader (
                    audio_format_manager.createReaderFor (path));

                auto num_channels = reader->numChannels;

                ir_metadata.title = ir_filename.stem ();
                ir_metadata.description = "Impulse response loaded from disk";
                ir_metadata.relative_path = file_path;
                ir_metadata.position_map = PositionMap {.centre = ""};

                ir_metadata.channel_format = GetChannelFormat (num_channels);
                ir_selection.target_format = GetValidTargetFormat (num_channels);

                context_.dispatch (LoadIrAction {ir_selection});
            }
        });
}

ChannelFormat LoadFromDiskController::GetChannelFormat (int num_channels)
{
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

TargetFormat LoadFromDiskController::GetValidTargetFormat (int num_channels)
{
    auto valid_target_formats = valid_target_formats_reader_.get ();

    for (auto & target_format : valid_target_formats)
    {
        if (num_channels = GetNumChannels (target_format))
            return target_format;
    }
}