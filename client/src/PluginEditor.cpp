#include "PluginEditor.h"

#include <ranges>

extern "C" const char ui_build_webview_files_zip [];
extern "C" const unsigned ui_build_webview_files_zip_size;

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
    auto assets_dir = juce::File (); // For running tests only! - should working on packaging for
                                     // Linux at some point.
    #warning "We only support Mac and Windows here yet."
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
    jassert (kBytesRead == result.size ());
    return result;
}

std::vector<std::byte> GetWebViewFileAsBytes (const juce::String & filepath)
{
    juce::MemoryInputStream zipStream {
        &ui_build_webview_files_zip, ui_build_webview_files_zip_size, false};
    juce::ZipFile zipFile {zipStream};

    if (auto * zipEntry = zipFile.getEntry (ZIPPED_FILES_PREFIX + filepath))
    {
        const std::unique_ptr<juce::InputStream> entryStream {
            zipFile.createStreamForEntry (*zipEntry)};

        if (entryStream == nullptr)
        {
            jassertfalse;
            return {};
        }

        return StreamToVector (*entryStream);
    }

    return {};
}
static std::vector<std::string> split (const std::string & s, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss (s);
    std::string item;

    while (std::getline (ss, item, delimiter))
    {
        tokens.push_back (item);
    }
    return tokens;
}
static std::optional<std::pair<std::string, std::string>>
ParseWebZoneImageURLScheme (const std::string & url)
{
    auto parts = split (url, '/');

    if (parts.size () != 4)
        return std::nullopt;

    if (parts [1] != "web_zone_image")
        return std::nullopt;

    return std::make_pair (parts [2], parts [3]);
}

std::optional<juce::WebBrowserComponent::Resource>
AudioPluginAudioProcessorEditor::GetResource (const juce::String & url)
{
    if (url == "/visualiser.bin")
        return visualiser_relay_.GetVisualiserResource ();

    if (url.startsWith ("/web_zone_image"))
    {
        auto request_parts = ParseWebZoneImageURLScheme (url.toStdString ());
        if (! request_parts)
            return std::nullopt;
        auto [zone_id, image_id] = *request_parts;
        return web_zones_relay_.GetWebZoneImageResource (zone_id, image_id);
    }

    const auto resourceToRetrieve =
        url == "/" ? "index.html" : url.fromFirstOccurrenceOf ("/", false, false);

    const auto resource = GetWebViewFileAsBytes (resourceToRetrieve);
    if (! resource.empty ())
    {
        const auto extension = resourceToRetrieve.fromLastOccurrenceOf (".", true, false);
        return juce::WebBrowserComponent::Resource {std::move (resource),
                                                    GetMimeForExtension (extension)};
    }

    return std::nullopt;
}

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (
    AudioPluginAudioProcessor & processor,
    ProcessorContainer & processor_container)
    : AudioProcessorEditor (&processor)
    , processor_ (processor)
    , processor_container_ (processor_container)
    , parameter_relay_ (web_browser_component_)
    , engine_relay_ (web_browser_component_,
                     processor_container.ir_engine_,
                     processor_container.convolution_engine_)
    , player_relay_ (web_browser_component_, processor_container.player_controller_)
    , preferences_relay_ (web_browser_component_, processor_container.preferences_controller_)
    , user_zones_relay_ (web_browser_component_)
    , load_relay_ (web_browser_component_, processor_container.load_controller_)
    , metering_relay_ (processor_container.input_graph_metering_,
                       processor_container.output_graph_metering_)
    , visualiser_relay_ (web_browser_component_, processor_container.visualiser_controller_)
    , web_zones_relay_ (web_browser_component_, processor_container.load_controller_)
    , web_browser_component_ (kBaseWebOptions.withOptionsFrom (parameter_relay_)
                                  .withOptionsFrom (engine_relay_)
                                  .withOptionsFrom (player_relay_)
                                  .withOptionsFrom (preferences_relay_)
                                  .withOptionsFrom (user_zones_relay_)
                                  .withOptionsFrom (load_relay_)
                                  .withOptionsFrom (metering_relay_)
                                  .withOptionsFrom (resize_relay_)
                                  .withOptionsFrom (visualiser_relay_)
                                  .withOptionsFrom (web_zones_relay_))
    , parameter_attachments_ (parameter_relay_, processor_container.parameter_tree_state_)

{
    setResizable (true, false);
    setResizeLimits (kWindowMinimumWidth,
                     static_cast<int> (kWindowMinimumWidth * kPreferredAspectRatio),
                     kWindowMaxWidth,
                     static_cast<int> (kWindowMaxWidth * kPreferredAspectRatio));
    setSize (1100, 600);
    resize_relay_.Setup (this, getConstrainer ());

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