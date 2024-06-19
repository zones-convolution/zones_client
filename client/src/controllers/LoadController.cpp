#include "LoadController.h"

const juce::String LoadController::kFilePickerDialogTitle = "Load IR from disk";

LoadController::LoadController (juce::ThreadPool & thread_pool, IrController & ir_controller)
    : thread_pool_ (thread_pool)
    , ir_controller_ (ir_controller)
{
}

void LoadController::Load (const IrSelection & ir_selection,
                           const std::function<void (bool)> & callback)
{
    SetLoadingIr (ir_selection);

    thread_pool_.addJob (
        [&, ir_selection, callback]
        {
            try
            {
                ir_controller_.LoadIr (ir_selection);
                callback (true);
            }
            catch (...)
            {
                callback (false);
            }

            SetLoadingIr (std::nullopt);
        });
}

static ChannelFormat GetChannelFormat (int num_channels)
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

static TargetFormat GetValidTargetFormat (int num_channels,
                                          const std::vector<TargetFormat> & valid_target_formats)
{
    for (auto & target_format : valid_target_formats)
    {
        if (num_channels = GetNumChannels (target_format))
        {
            if (target_format == TargetFormat::kQuadraphonic)
                continue;
            return target_format;
        }
    }
}

void LoadController::LoadFromDisk ()
{
    file_picker_ = std::make_unique<juce::FileChooser> (kFilePickerDialogTitle);
    auto directory_flags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    file_picker_->launchAsync (
        directory_flags,
        [&] (const juce::FileChooser & chooser)
        {
            auto path = chooser.getResult ();
            if (path.exists ())
            {
                juce::MessageManager::callAsync (
                    [&, path]
                    {
                        IrSelection ir_selection;
                        auto & ir_metadata = ir_selection.ir;

                        auto file_path =
                            std::filesystem::path {path.getFullPathName ().toStdString ()};
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
                        ir_selection.target_format =
                            GetValidTargetFormat (num_channels, valid_target_formats_);
                        Load (ir_selection, [] (bool result) {});
                    });
            }
        });
}

const std::optional<IrSelection> & LoadController::GetLoadingIr ()
{
    std::lock_guard guard {loading_ir_mutex_};
    return loading_ir_;
}

void LoadController::SetLoadingIr (const std::optional<IrSelection> & ir_selection)
{
    loading_ir_mutex_.lock ();
    loading_ir_ = ir_selection;
    loading_ir_mutex_.unlock ();
    OnLoadingIrUpdated ();
}
