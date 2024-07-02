#include "PluginEditor.h"

const juce::String AudioPluginAudioProcessorEditor::kLocalDevServerAddress =
    "http://localhost:5173/";

bool AudioPluginAudioProcessorEditor::SinglePageBrowser::pageAboutToLoad (
    const juce::String & newURL)
{
    return newURL == AudioPluginAudioProcessorEditor::kLocalDevServerAddress ||
           newURL == getResourceProviderRoot ();
}

static juce::File GetAssetsDirectory ()
{
#if JUCE_MAC
    auto assets_dir =
        juce::File::getSpecialLocation (juce::File::SpecialLocationType::currentApplicationFile)
            .getChildFile ("Contents/Resources/dist");
#elif JUCE_WINDOWS
    auto assets_dir = juce::File::getSpecialLocation (
                          juce::File::SpecialLocationType::
                              currentExecutableFile) // Plugin.vst3/Contents/<arch>/Plugin.vst3
                          .getParentDirectory ()     // Plugin.vst3/Contents/<arch>/
                          .getParentDirectory ()     // Plugin.vst3/Contents/
                          .getChildFile ("Resources/dist");
#else
    #error "We only support Mac and Windows here yet."
#endif

    return assets_dir;
}

static const char * GetMimeForExtension (const juce::String & extension)
{
    static const std::unordered_map<juce::String, const char *> mime_map = {
        {{".htm"}, "text/html"},
        {{".html"}, "text/html"},
        {{".txt"}, "text/plain"},
        {{".jpg"}, "image/jpeg"},
        {{".jpeg"}, "image/jpeg"},
        {{".svg"}, "image/svg+xml"},
        {{".ico"}, "image/vnd.microsoft.icon"},
        {{".json"}, "application/json"},
        {{".png"}, "image/png"},
        {{".css"}, "text/css"},
        {{".map"}, "application/json"},
        {{".js"}, "text/javascript"},
        {{".woff2"}, "font/woff2"}};

    if (const auto it = mime_map.find (extension.toLowerCase ()); it != mime_map.end ())
        return it->second;

    jassertfalse;
    return "";
}

static auto StreamToVector (juce::InputStream & stream)
{
    std::vector<std::byte> result ((size_t) stream.getTotalLength ());
    stream.setPosition (0);
    [[maybe_unused]] const auto kBytesRead = stream.read (result.data (), result.size ());
    jassert (kBytesRead == (ssize_t) result.size ());
    return result;
}

std::optional<juce::WebBrowserComponent::Resource>
AudioPluginAudioProcessorEditor::GetResource (const juce::String & url)
{
    auto rel_path = "." + (url == "/" ? "/index.html" : url);
    auto asset_file = asset_directory_.getChildFile (rel_path);

    if (! asset_file.existsAsFile ())
        return std::nullopt;

    auto file_stream = asset_file.createInputStream ();
    return juce::WebBrowserComponent::Resource {
        StreamToVector (*file_stream), GetMimeForExtension (asset_file.getFileExtension ())};
}

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (
    AudioPluginAudioProcessor & processor,
    ProcessorContainer & processor_container)
    : AudioProcessorEditor (&processor)
    , processor_ (processor)
    , processor_container_ (processor_container)
    , engine_relay_ (web_browser_component_,
                     processor_container.ir_engine_,
                     processor_container.convolution_engine_)
    , player_relay_ (web_browser_component_, processor_container.player_controller_)
    , preferences_relay_ (web_browser_component_, preferences_controller_)
    , user_zones_relay_ (web_browser_component_)
    , load_relay_ (web_browser_component_, processor_container.load_controller_)
    , metering_relay_ (processor_container.input_graph_metering_,
                       processor_container.output_graph_metering_)
    , web_browser_component_ (kBaseWebOptions.withOptionsFrom (wet_dry_mix_relay_)
                                  .withOptionsFrom (resampler_relay_)
                                  .withOptionsFrom (room_size_relay_)
                                  .withOptionsFrom (engine_relay_)
                                  .withOptionsFrom (player_relay_)
                                  .withOptionsFrom (preferences_relay_)
                                  .withOptionsFrom (user_zones_relay_)
                                  .withOptionsFrom (load_relay_)
                                  .withOptionsFrom (metering_relay_))
    , wet_dry_mix_attachment_ (
          *processor_container_.parameter_tree_.getParameter (ParameterTree::kDryWetMixParameterId),
          wet_dry_mix_relay_)
    , resampler_attachment_ (
          *processor_container_.parameter_tree_.getParameter (ParameterTree::kResamplerParameterId),
          resampler_relay_)
    , room_size_attachment_ (
          *processor_container_.parameter_tree_.getParameter (ParameterTree::kRoomSizeParameterId),
          room_size_relay_)

{
    setResizable (true, true);
    setResizeLimits (kWindowMinimumWidth,
                     static_cast<int> (kWindowMinimumWidth * kPreferredAspectRatio),
                     kWindowMaxWidth,
                     static_cast<int> (kWindowMaxWidth * kPreferredAspectRatio));
    setSize (1280, 600);

    asset_directory_ = GetAssetsDirectory ();

#if DEV_LOCALHOST
    web_browser_component_.goToURL (kLocalDevServerAddress);
#else
    web_browser_component_.goToURL (juce::WebBrowserComponent::getResourceProviderRoot ());
#endif

    addAndMakeVisible (web_browser_component_);
}

void AudioPluginAudioProcessorEditor::resized ()
{
    web_browser_component_.setBounds (getLocalBounds ());
}