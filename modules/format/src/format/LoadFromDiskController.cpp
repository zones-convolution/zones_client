#include "LoadFromDiskController.h"

LoadFromDiskController::LoadFromDiskController (lager::context<Action> & context)
    : context_ (context)
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

ChannelFormat LoadFromDiskController::GetChannelFormatFromFile (juce::File & path)
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